# AC Reports - Geotab Add-In

A custom Geotab add-in web application for creating and customizing reports for exceptions, trips, and faults.

## Overview

AC Reports is a Geotab add-in that allows users to create customized reports from Geotab data. The application offers three types of reports:

1. **Exceptions Reports** - Create custom exception reports
2. **Trips Reports** - Generate detailed trip reports
3. **Faults Reports** - Build diagnostic fault reports

## Features

### Common Features Across All Report Types
- **Asset/Driver Selection** - Choose specific assets or drivers to include in your reports
- **Group Filtering** - Filter assets/drivers by groups for easier selection
- **Custom Column Selection** - Choose which data columns to display in your report
- **Column Order Customization** - Arrange columns in your preferred order
- **Sorting Options** - Select which column to sort your data by
- **Dashboard Interface** - Central hub to select between report types
- **Report Export** - Download reports in various formats

### Report-Specific Features
- **Exceptions Report** - Select specific exceptions to include
- **Faults Report** - Choose diagnostics to include in your report

## Technical Architecture

### Backend
- Flask-based Python application
- Geotab API integration via mygeotab SDK
- Data processing with pandas

### Frontend
- HTML/JavaScript interface
- Bootstrap 5 for responsive design
- Custom report building functionality

## Installation

### Prerequisites
- Python 3.6+
- Geotab account with API access
- Web server for hosting the application

### Setup

1. Clone the repository
```bash
git clone https://github.com/parkerhendry/projects.git
cd projects/Geotab/[project-folder]
```

2. Install dependencies
```bash
pip install -r requirements.txt
```

3. Configure your Geotab credentials (follow security best practices)

4. Run the application
```bash
python ACreport.py
```

## Usage

1. Access the dashboard through your Geotab interface or directly via the hosted URL
2. Select the type of report you wish to create (Exceptions, Trips, or Faults)
3. Configure your report parameters:
   - Select assets/drivers (filter by groups if needed)
   - For Exceptions reports: Choose specific exceptions to include
   - For Faults reports: Select diagnostics to include
   - Choose data columns to display
   - Arrange columns in your preferred order
   - Set your sort preference
4. Generate and download your report

## Files Overview

### Backend
- `ACreport.py` - Main Python backend file handling API requests and data processing

### Frontend
- `ACreport.html/js` - Main application interface
- `build-report.html/js` - Core report building functionality
- `trips-report.html/js` - Trips report specific interface
- `faults-report.html/js` - Faults report specific interface
- `exceptions-report.html/js` - Exceptions report specific interface (implied from your description)

## Development

To contribute to this project:

1. Fork the repository
2. Create a feature branch
3. Submit a pull request