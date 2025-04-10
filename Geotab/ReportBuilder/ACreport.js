document.addEventListener("DOMContentLoaded", function () {
    const generateReportBtn = document.getElementById("generateReportBtn");
    const statusMessage = document.getElementById("statusMessage");
    const loadingOverlay = document.getElementById("loadingOverlay");
    const loadingMessage = document.getElementById("loadingMessage");

    function showLoading(message) {
        loadingMessage.textContent = message || "Processing...";
        loadingOverlay.style.display = "flex";
    }

    function hideLoading() {
        loadingOverlay.style.display = "none";
    }

    function updateStatus(message, isError = false) {
        statusMessage.innerHTML = `<div class="alert ${isError ? "alert-danger" : "alert-info"}">${message}</div>`;
    }

    function checkReportStatus() {
        fetch("/api/report-status")
            .then(response => {
                if (!response.ok) {
                    throw new Error(`Server responded with status: ${response.status}`);
                }
                return response.json();
            })
            .then(data => {
                // Update loading message with current status
                if (data.message) {
                    loadingMessage.textContent = data.message;
                }
                
                if (data.status === "complete") {
                    hideLoading();
                    updateStatus("Report generation complete. Downloading now...");
                    // Small delay to ensure the user sees the success message
                    setTimeout(() => {
                        window.location.href = "/api/download-report";
                    }, 1000);
                } else if (data.status === "error") {
                    hideLoading();
                    updateStatus(`Error: ${data.message}`, true);
                } else {
                    // Continue checking status
                    setTimeout(checkReportStatus, 2000);
                }
            })
            .catch(error => {
                hideLoading();
                updateStatus(`Error checking status: ${error.message}`, true);
                console.error("Error checking report status:", error);
            });
    }

    generateReportBtn.addEventListener("click", function () {
        // Clear previous status
        statusMessage.innerHTML = "";
        
        // Show loading overlay
        showLoading("Starting report generation...");
        
        // Disable button to prevent multiple clicks
        generateReportBtn.disabled = true;
        
        fetch("/api/generate-report", { 
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            }
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error(`Server responded with status: ${response.status}`);
                }
                return response.json();
            })
            .then(data => {
                if (data.status === "processing") {
                    updateStatus("Report generation started...");
                    checkReportStatus();
                } else {
                    hideLoading();
                    updateStatus(`Error: ${data.message}`, true);
                    // Re-enable button
                    generateReportBtn.disabled = false;
                }
            })
            .catch(error => {
                hideLoading();
                updateStatus(`Error: ${error.message}`, true);
                console.error("Error starting report generation:", error);
                // Re-enable button
                generateReportBtn.disabled = false;
            });
    });
});