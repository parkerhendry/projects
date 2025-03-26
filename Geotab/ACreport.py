import mygeotab
import getpass
import requests
import pandas as pd
import datetime
import os
import time
import smtplib
import schedule
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.utils import formatdate
from email import encoders
from flask import Flask, jsonify, send_file
from functools import lru_cache
from threading import Thread
from dateutil import parser

app = Flask(__name__, static_folder='.', static_url_path='')

# Global variables to track report generation status
report_status = {"status": "idle", "message": ""}
report_file_path = "speed_violations_report.xlsx"

# Email configuration
EMAIL_HOST = "smtp.office365.com"  # Replace with your SMTP server
EMAIL_PORT = 587
EMAIL_USER = "email@email.com"  # Replace with your email
EMAIL_PASSWORD = getpass.getpass("Enter your email password: ")
EMAIL_RECIPIENTS = ["recipient@email.com"]  # List of recipients
GEO_PASSWORD = getpass.getpass("Enter your Geotab password: ")
GEO_DATABASE = "database"  # Replace with your Geotab database name

@lru_cache(maxsize=None)
def decode_vin(vin: str) -> dict:
    """Decode VIN to get vehicle make, model, and year."""
    if not vin or vin == "Unknown":
        return {"make": "Unknown", "model": "Unknown", "year": "Unknown"}
    
    url = f"https://vpic.nhtsa.dot.gov/api/vehicles/DecodeVin/{vin}?format=json"
    response = requests.get(url)
    
    if response.status_code != 200:
        return {"make": "Unknown", "model": "Unknown", "year": "Unknown"}
        
    data = response.json()
    results = data.get("Results", [])
    
    return {
        "year": next((item["Value"] for item in results if item["Variable"] == "Model Year" and item["Value"]), "Unknown"),
        "make": next((item["Value"] for item in results if item["Variable"] == "Make" and item["Value"]), "Unknown"),
        "model": next((item["Value"] for item in results if item["Variable"] == "Model" and item["Value"]), "Unknown")
    }

def authenticate_geotab():
    """Authenticate with the MyGeotab API."""
    username = EMAIL_USER
    database = GEO_DATABASE
    
    # For production use getpass, for development/testing use a hardcoded password
    # In production environment, use environment variables or secure password storage
    #try:
    password = GEO_PASSWORD
    #except Exception:
        # When running as a service, use environment variable
        #password = os.environ.get("GEOTAB_PASSWORD", "")
        
    # Initialize MyGeotab API
    api = mygeotab.API(username=username, password=password, database=database)
    api.authenticate()
    print("Authenticated with Geotab successfully.")
    return api

def convert_datetime(dt_str):
    """Convert datetime string to a user-friendly format."""
    if not dt_str or dt_str == "Unknown":
        return "Unknown"
    
    try:
        # Handle datetime string
        if isinstance(dt_str, str):
            dt_obj = parser.parse(dt_str)
        else:
            dt_obj = dt_str  # Already a datetime object
            
        # Format as a user-friendly string (MM/DD/YYYY HH:MM:SS)
        return dt_obj.strftime("%m/%d/%Y %I:%M:%S %p")
    except Exception as e:
        print(f"Error converting datetime {dt_str}: {e}")
        return "Unknown"

def format_duration(duration_value):
    """Format duration to a readable format (HH:MM:SS)."""
    if not duration_value or duration_value == "Unknown":
        return "Unknown"
    
    try:
        # If it's a datetime.time object
        if hasattr(duration_value, 'hour') and hasattr(duration_value, 'minute') and hasattr(duration_value, 'second'):
            return f"{duration_value.hour:02d}:{duration_value.minute:02d}:{duration_value.second:02d}"
        
        # If duration is a string like "00:01:00.823000"
        if isinstance(duration_value, str) and ":" in duration_value:
            parts = duration_value.split(":")
            if len(parts) == 3:
                hours = int(parts[0])
                minutes = int(parts[1])
                # Handle seconds that might have milliseconds
                seconds_parts = parts[2].split(".")
                seconds = int(seconds_parts[0])
                return f"{hours:02d}:{minutes:02d}:{seconds:02d}"
        
        # If it's a number of seconds
        if isinstance(duration_value, (int, float)) or (isinstance(duration_value, str) and duration_value.replace('.', '', 1).isdigit()):
            duration_seconds = float(duration_value)
            hours = int(duration_seconds // 3600)
            minutes = int((duration_seconds % 3600) // 60)
            seconds = int(duration_seconds % 60)
            return f"{hours:02d}:{minutes:02d}:{seconds:02d}"
            
        # If we can't determine the format, just return as string
        return str(duration_value)
    except Exception as e:
        print(f"Error formatting duration {duration_value}: {e}")
        return "Unknown"

def format_distance(distance_km):
    """Format distance in kilometers to miles with 2 decimal places."""
    if not distance_km or distance_km == "Unknown":
        return "Unknown"
    
    try:
        # Convert to float if it's not already
        distance_km = float(distance_km)
        
        # Format with 2 decimal places
        return f"{distance_km:.2f}"
    except Exception:
        return "Unknown"

def send_email(subject, body, file_path=None, html_content=None):
    """Send an email with optional attachment and HTML content."""
    try:
        msg = MIMEMultipart('alternative')
        msg['From'] = EMAIL_USER
        msg['To'] = ", ".join(EMAIL_RECIPIENTS)
        msg['Date'] = formatdate(localtime=True)
        msg['Subject'] = subject
        
        # Attach plain text version
        msg.attach(MIMEText(body, 'plain'))
        
        # Attach HTML version if provided
        if html_content:
            msg.attach(MIMEText(html_content, 'html'))
        
        # Attach file if provided
        if file_path and os.path.exists(file_path):
            attachment = MIMEBase('application', 'octet-stream')
            with open(file_path, 'rb') as file:
                attachment.set_payload(file.read())
            encoders.encode_base64(attachment)
            attachment.add_header('Content-Disposition', 
                               f'attachment; filename="{os.path.basename(file_path)}"')
            msg.attach(attachment)
        
        # Connect to server and send email
        with smtplib.SMTP(EMAIL_HOST, EMAIL_PORT) as server:
            server.starttls()
            server.login(EMAIL_USER, EMAIL_PASSWORD)
            server.send_message(msg)
            
        print(f"Email sent successfully to {', '.join(EMAIL_RECIPIENTS)}")
        return True
    except Exception as e:
        print(f"Failed to send email: {e}")
        return False

def generate_report_with_dates(from_date, to_date, custom_filename=None):
    """Generate report for specified date range."""
    global report_status, report_file_path
    
    if custom_filename:
        report_file_path = custom_filename
    
    try:
        report_status = {"status": "processing", "message": "Authenticating with Geotab..."}
        api = authenticate_geotab()
        
        # Combine all exception calls into a single list
        rule_ids = [
            "a_xO6WGKe7EaAcWCoMd4CMQ",
            "alctVH7Bq60qLxmQsfLs7bg",
            "aawYjABMTDk-D-HrlplkojA"
        ]

        rule_names = {
            "a_xO6WGKe7EaAcWCoMd4CMQ": "Posted Speed Limit Violation",
            "alctVH7Bq60qLxmQsfLs7bg": "Excessive posted speed limit violation",
            "aawYjABMTDk-D-HrlplkojA": "Vehicle speed over 75mph"
        }

        cost_center_map = {
            "2091410330": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091410430": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091410530": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091410630": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091410730": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091410830": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091411030": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091411130": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091411230": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091411430": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091418830": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091419930": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091412230": {"region": "EAST", "operations_manager": "DANIEL LANE"},
            "2091420430": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091420530": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091420630": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091420730": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091420830": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091420930": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091421030": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091421130": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091421230": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091421330": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091421430": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091421530": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091423330": {"region": "SOUTH", "operations_manager": "ALBERTO DUARTE"},
            "2091430330": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091430430": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091430530": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091430630": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091430730": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091430830": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091431030": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091431130": {"region": "CENTRAL", "operations_manager": "TIM HOBAN"},
            "2091440530": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091440630": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091440730": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091440830": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091440930": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091441030": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091441130": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091444430": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091445530": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"},
            "2091447770": {"region": "WEST", "operations_manager": "JACKIE ARRIBERE"}
        }

        report_status = {"status": "processing", "message": "Fetching exception data..."}
        
        # Fetch exceptions for all rule IDs
        exceptions = []
        for rule_id in rule_ids:
            exceptions.extend(api.call("Get", typeName="ExceptionEvent", search={
                "fromDate": from_date,
                "toDate": to_date,
                "ruleSearch": {"id": rule_id}
            }))

        # Extract unique device IDs from exceptions
        device_ids = set(exception['device']['id'] for exception in exceptions)

        report_status = {"status": "processing", "message": "Fetching device data..."}
        
        # Fetch all devices in a single API call
        all_devices = api.call("Get", typeName="Device")

        # Map device IDs to device details
        device_map = {device['id']: device for device in all_devices if device['id'] in device_ids}

        # Fetch DeviceStatusInfo in a single API call
        device_status_info = api.call("Get", typeName="DeviceStatusInfo")

        # Map device IDs to DeviceStatusInfo
        device_status_map = {status['device']['id']: status for status in device_status_info}

        report_status = {"status": "processing", "message": "Fetching user data..."}
        
        # Fetch all users in a single API call
        all_users = api.call("Get", typeName="User")

        # Map user IDs to user details
        user_map = {user['id']: user for user in all_users}

        # Fetch all groups in a single API call
        all_groups = api.call("Get", typeName="Group")

        # Map group IDs to group names
        group_map = {group['id']: group['name'] for group in all_groups}

        report_status = {"status": "processing", "message": "Processing exception data..."}
        
        # Process each exception to gather required details
        processed_exceptions = []
        count = 0
        
        for exception in exceptions:
            count += 1
            device_id = exception['device']['id']
            device = device_map.get(device_id, {})
            
            # Get device fields
            name = device.get("name", "Unknown")
            vin = device.get("vehicleIdentificationNumber", "Unknown")
            
            # Decode VIN
            vin_details = decode_vin(vin)
            make = vin_details.get("make", "Unknown")
            model = vin_details.get("model", "Unknown")
            year = vin_details.get("year", "Unknown")

            # Get Device Group details
            device_group_id = device.get("groups", [{}])[0].get("id", "Unknown")
            deviceGroup = group_map.get(device_group_id, "Unknown")
            
            # Get DeviceStatusInfo
            device_status = device_status_map.get(device_id, {})
            driver = device_status.get("driver", {})
            if not isinstance(driver, dict):
                driver = {}
            driverID = driver.get("id", "Unknown")
            
            # Get User details
            user = user_map.get(driverID, {})
            firstName = user.get("firstName", "Unknown")
            lastName = user.get("lastName", "Unknown")
            
            # Get Group details
            groupID = user.get("driverGroups", [{}])[0].get("id", "Unknown")
            userGroup = group_map.get(groupID, "Unknown")

            # Extract cost center from userGroup
            cost_center = userGroup.split(" - ")[0] if " - " in userGroup else "Unknown"
            
            # Get region and operations manager from cost center map
            cost_center_details = cost_center_map.get(cost_center, {"region": "Unknown", "operations_manager": "Unknown"})
            region = cost_center_details["region"]
            operationsManager = cost_center_details["operations_manager"]
            
            # Handle datetime and duration properly
            activeFrom = convert_datetime(exception.get("activeFrom", "Unknown"))
            activeTo = convert_datetime(exception.get("activeTo", "Unknown"))
            
            # Direct approach for duration using the duration field
            try:
                # Try to directly use the duration field if available
                duration_str = str(exception.get("duration", "Unknown"))
                duration = format_duration(duration_str)
                
                # If this didn't work and we have activeFrom and activeTo, calculate duration
                if duration == "Unknown" and exception.get("activeFrom") and exception.get("activeTo"):
                    try:
                        from_dt = parser.parse(str(exception.get("activeFrom")))
                        to_dt = parser.parse(str(exception.get("activeTo")))
                        duration_seconds = (to_dt - from_dt).total_seconds()
                        duration = format_duration(duration_seconds)
                    except Exception as e:
                        print(f"Error calculating duration from timestamps: {e}")
            except Exception as e:
                print(f"Error handling duration: {e}")
                duration = "Unknown"
                
            # Format distance
            distance = format_distance(exception.get("distance", "Unknown"))

            # Get Rule Name using exception["rule"]["id"]
            rule_id = exception.get("rule", {}).get("id", "Unknown")
            ruleName = rule_names.get(rule_id, "Unknown Rule")
            
            # Add to processed exceptions - excluding VIN, device comment, and driver comment
            processed_exceptions.append({
                "Vehicle": name,
                "Make": make,
                "Model": model,
                "Year": year,
                "Device Group": deviceGroup,
                "Driver First Name": firstName,
                "Driver Last Name": lastName,
                "User Group": userGroup,
                "Cost Center": cost_center,
                "Region": region,
                "Operations Manager": operationsManager,
                "Violation Type": ruleName,
                "Start Time": activeFrom,
                "End Time": activeTo,
                "Duration": duration,
                "Distance (Km)": distance
            })

        report_status = {"status": "processing", "message": "Creating Excel report..."}
        
        # Create DataFrame from processed exceptions
        df = pd.DataFrame(processed_exceptions)
        
        # Generate Excel file
        with pd.ExcelWriter(report_file_path, engine='openpyxl') as writer:
            df.to_excel(writer, index=False, sheet_name="Speed Violations")
            
            # Auto-adjust column widths
            worksheet = writer.sheets["Speed Violations"]
            for i, col in enumerate(df.columns):
                max_length = max(df[col].astype(str).map(len).max(), len(col)) + 2
                worksheet.column_dimensions[chr(65 + i)].width = min(max_length, 50)  # Limit max width to 50
            
            # Extract date from from_date for report
            try:
                report_date = parser.parse(from_date).strftime("%Y-%m-%d")
            except:
                report_date = datetime.datetime.now().strftime("%Y-%m-%d")
                
            worksheet.cell(row=1, column=len(df.columns) + 2).value = f"Report Date: {report_date}"
        
        report_status = {"status": "complete", "message": "Report generation complete"}
        
        # Return the count of violations for reporting
        return len(processed_exceptions)
        
    except Exception as e:
        report_status = {"status": "error", "message": str(e)}
        print(f"Error generating report: {e}")
        return 0

def generate_report_task():
    """Background task to generate the report."""
    global report_status
    
    try:
        # Get yesterday's date
        yesterday = datetime.datetime.now() - datetime.timedelta(days=1)
        from_date = yesterday.strftime("%Y-%m-%dT00:00:00.000Z")
        to_date = (yesterday + datetime.timedelta(days=1)).strftime("%Y-%m-%dT00:00:00.000Z")
        
        # Generate the report
        generate_report_with_dates(from_date, to_date)
        
    except Exception as e:
        report_status = {"status": "error", "message": str(e)}
        print(f"Error generating report: {e}")

def scheduled_daily_report():
    """Function to be scheduled to run daily - generates and emails the report."""
    print(f"Running scheduled daily report at {datetime.datetime.now()}")
    
    # Calculate previous day's date range
    yesterday = datetime.datetime.now() - datetime.timedelta(days=1)
    yesterday_date = yesterday.strftime("%Y-%m-%d")
    
    from_date = f"{yesterday_date}T00:00:00.000Z"
    to_date = f"{yesterday_date}T23:59:59.999Z"
    
    # Create a filename with the date
    filename = f"speed_violations_{yesterday_date}.xlsx"
    
    # Generate the report
    violations_count = generate_report_with_dates(from_date, to_date, filename)
    
    # Plain text email
    email_subject = f"Daily Speed Violations Report - {yesterday_date}"
    email_body = f"""
Hello,

Attached is the daily speed violations report for {yesterday_date}.

Summary:
- Total violations: {violations_count}
- Date range: {yesterday_date}
- Generated on: {datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")}

This is an automated email from the Traxxis reports system.
    """
    
    # HTML version of the email with enhanced formatting using company colors
    html_content = f"""
    <html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
            body {{
                font-family: 'Segoe UI', Arial, sans-serif;
                line-height: 1.6;
                margin: 0;
                padding: 0;
                background-color: #f9f9f9;
                color: #333333;
            }}
            .container {{
                max-width: 650px;
                margin: 0 auto;
                background-color: #ffffff;
                border-radius: 8px;
                overflow: hidden;
                box-shadow: 0 4px 10px rgba(0,0,0,0.1);
            }}
            .header {{
                background-color: #6bc6db;
                padding: 25px;
                text-align: center;
            }}
            .header h2 {{
                color: #ffffff;
                margin-bottom: 5px;
                font-weight: 600;
            }}
            .header p {{
                color: #ffffff;
                margin-top: 0;
                opacity: 0.9;
            }}
            .logo-container {{
                margin-bottom: 15px;
                display: inline-block;
                background-color: white;
                padding: 10px;
                border-radius: 6px;
                /* Add a border to ensure consistent background */
                border: 1px solid #e0e0e0;
            }}
            .logo {{
                height: 60px;
                display: block;
            }}
            .content {{
                padding: 30px;
                color: #333333;
            }}
            .summary {{
                background-color: #f5f5f5;
                padding: 20px;
                margin-top: 25px;
                border-radius: 6px;
                border-left: 4px solid #f5844a;
            }}
            .summary h3 {{
                margin-top: 0;
                color: #f5844a;
                font-weight: 600;
            }}
            .summary ul {{
                padding-left: 20px;
                margin-bottom: 0;
            }}
            .summary li {{
                margin-bottom: 8px;
            }}
            .summary li:last-child {{
                margin-bottom: 0;
            }}
            .footer {{
                font-size: 12px;
                color: #777777;
                margin-top: 30px;
                background-color: #f0f0f0;
                padding: 15px;
                text-align: center;
                border-top: 3px solid #6bc6db;
            }}
            .contact {{
                margin-top: 25px;
                padding-top: 15px;
                border-top: 1px solid #eaeaea;
            }}
            @media screen and (max-width: 600px) {{
                .container {{
                    width: 100%;
                    border-radius: 0;
                }}
                .content {{
                    padding: 20px;
                }}
            }}
        </style>
    </head>
    <body>
        <div class="container">
            <div class="header">
                <div class="logo-container">
                    <img class="logo" src="https://traxxisgps.com/wp-content/uploads/elementor/thumbs/Traxxis-refresh-logo_horizontal-min-1-qjgvd5cr9kxu5eay6trn10pbylz31ardqnqdluuew0.webp" alt="Traxxis GPS Logo">
                </div>
                <h2>Daily Speed Violations Report</h2>
                <p>{yesterday_date}</p>
            </div>
            <div class="content">
                <p>Hello,</p>
                <p>The daily speed violations report for <strong>{yesterday_date}</strong> has been generated and is attached to this email.</p>
                
                <div class="summary">
                    <h3>Report Summary</h3>
                    <ul>
                        <li><strong>Total violations:</strong> {violations_count}</li>
                        <li><strong>Date range:</strong> {yesterday_date}</li>
                        <li><strong>Generated on:</strong> {datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")}</li>
                    </ul>
                </div>
                
                <p style="margin-top: 25px;">Please review the attached Excel file for detailed information about all speed violations that occurred yesterday.</p>
                
                <div class="contact">
                    <p>If you have any questions about this report, please contact <a href="mailto:parker@traxxisgps.com" style="color: #6bc6db; text-decoration: none;">parker@traxxisgps.com</a></p>
                </div>
            </div>
            <div class="footer">
                <p>&copy; {datetime.datetime.now().year} Traxxis GPS. All rights reserved.</p>
                <p>This is an automated email from the Traxxis reports system. Please do not reply to this email.</p>
            </div>
        </div>
    </body>
    </html>
    """
    
    send_email(email_subject, email_body, filename, html_content)
    
    print(f"Daily report completed with {violations_count} violations.")

def start_scheduler():
    """Start the scheduler for daily reports."""
    # Schedule the report to run daily at 5:00 AM
    schedule.every().day.at("11:50").do(scheduled_daily_report)
    
    print("Scheduler started. Will generate and email reports daily at 5:00 AM.")
    
    # Run the scheduler in a loop
    while True:
        schedule.run_pending()
        time.sleep(60)  # Check every minute

@app.route('/')
def home():
    return app.send_static_file('ACreport.html')

@app.route('/api/generate-report', methods=['POST'])
def generate_report():
    global report_status
    
    # Reset status
    report_status = {"status": "processing", "message": "Starting report generation..."}
    
    # Start report generation in a background thread
    thread = Thread(target=generate_report_task)
    thread.daemon = True
    thread.start()
    
    return jsonify(report_status)

@app.route('/api/report-status')
def check_status():
    return jsonify(report_status)

@app.route('/api/download-report')
def download_report():
    if report_status["status"] == "complete" and os.path.exists(report_file_path):
        return send_file(report_file_path, as_attachment=True)
    else:
        return jsonify({"status": "error", "message": "Report not ready or not found"}), 404

if __name__ == '__main__':
    # Only start the scheduler in the main process, not when Flask reloads
    import os
    if not os.environ.get('WERKZEUG_RUN_MAIN'):
        # Start the scheduler in a separate thread
        scheduler_thread = Thread(target=start_scheduler)
        scheduler_thread.daemon = True
        scheduler_thread.start()
    
    # Start the Flask app
    app.run(host='0.0.0.0', port=5000, debug=True)