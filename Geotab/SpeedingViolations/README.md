# Geotab Speed Violations Report

## Overview
This Geotab add-in web application retrieves speeding exceptions for each vehicle using the Geotab API. It processes the data and generates various reports, including:
- Speeding violations with additional vehicle details (e.g., VIN decoding)
- Mappings to cost centers
- Customized filtering and report formats

The application also includes:
- A **dashboard UI** built with Bootstrap to allow users to select and generate reports
- **Automated email reports** with stylish HTML formatting, scheduled for daily distribution

## Features
- Fetches speeding exceptions from Geotab
- Decodes VIN numbers using an external API
- Maps vehicles to cost centers and organizational data
- Provides a user-friendly dashboard for report selection
- Generates reports in **Excel format**
- **Email automation** for daily scheduled reports
- Uses Flask for backend processing
- Includes caching for performance optimization

## Technologies Used
- **Backend:** Python, Flask, Pandas, MyGeotab API
- **Frontend:** HTML, CSS, Bootstrap, JavaScript
- **Email Automation:** smtplib, MIME libraries
- **Data Processing:** Pandas, OpenPyXL
- **Additional Tools:** Requests, Schedule, Dateutil

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo-name.git
   cd geotab-speed-report
   ```
2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Configure environment variables for authentication (e.g., Geotab credentials).
4. Start the Flask server:
   ```bash
   python ACreport.py
   ```
5. Open `http://localhost:5000` in your browser to access the dashboard.

## Usage
- Navigate to the dashboard UI.
- Select the type of report to generate.
- Click **"Generate Report"**, and the system will process the data.
- Download the report once completed.
- Scheduled reports will be sent via email automatically.

## API Endpoints
- `POST /api/generate-report` – Triggers report generation
- `GET /api/report-status` – Checks report status
- `GET /api/download-report` – Downloads the generated report

## Future Enhancements
- Additional report types (e.g., idle time, route efficiency)
- Integration with cloud storage for report hosting
- Advanced filtering and customization options

---

