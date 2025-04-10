document.addEventListener('DOMContentLoaded', function() {
    const reportForm = document.getElementById('reportForm');
    const statusMessage = document.getElementById('statusMessage');
    const loadingOverlay = document.getElementById('loadingOverlay');
    const loadingMessage = document.getElementById('loadingMessage');
    const progressBar = document.querySelector('.progress-bar');

// Date range handling
document.querySelectorAll('input[name="dateRange"]').forEach(radio => {
    radio.addEventListener('change', (e) => {
        const customDateRange = document.getElementById('customDateRange');
        const fromDate = document.getElementById('fromDate');
        const toDate = document.getElementById('toDate');
        
        if (e.target.value === 'custom') {
            customDateRange.style.display = 'flex';
            return;
        }
        
        customDateRange.style.display = 'none';
        const dates = calculateDateRange(e.target.value);
        fromDate.value = dates.fromDate;
        toDate.value = dates.toDate;
    });
});

function calculateDateRange(range) {
    const today = new Date();
    const fromDate = new Date();
    const toDate = new Date();

    switch (range) {
        case 'today':
            break;
            
        case 'yesterday':
            fromDate.setDate(today.getDate() - 1);
            toDate.setDate(today.getDate() - 1);
            break;
            
        case 'thisWeek':
            fromDate.setDate(today.getDate() - today.getDay());
            break;
            
        case 'lastWeek':
            fromDate.setDate(today.getDate() - today.getDay() - 7);
            toDate.setDate(today.getDate() - today.getDay() - 1);
            break;
            
        case 'thisMonth':
            fromDate.setDate(1);
            break;
            
        case 'lastMonth':
            fromDate.setMonth(today.getMonth() - 1, 1);
            toDate.setMonth(today.getMonth(), 0);
            break;
    }

    return {
        fromDate: fromDate.toISOString().split('T')[0],
        toDate: toDate.toISOString().split('T')[0]
    };
}

// Set default selection to 'today'
document.getElementById('today').checked = true;
document.getElementById('fromDate').value = new Date().toISOString().split('T')[0];
document.getElementById('toDate').value = new Date().toISOString().split('T')[0];

// Update the devices loading section
let driverGroups = [];

// First, fetch driver groups
fetch('/api/driver-groups')
    .then(response => response.json())
    .then(data => {
        if (data.error) throw new Error(data.error);
        driverGroups = data;
        loadDevices();
    })
    .catch(error => {
        console.error('Error loading driver groups:', error);
        loadDevices(); // Still load devices even if groups fail
    });

let isDriverBased = false;

document.querySelectorAll('input[name="selectionType"]').forEach(radio => {
    radio.addEventListener('change', (e) => {
        isDriverBased = e.target.value === 'driver';
        loadDevices([], isDriverBased ? '/api/drivers' : '/api/devices');
    });
});

// Modify your existing loadDevices function slightly to accept an endpoint parameter
function loadDevices(selectedGroups = [], endpoint = '/api/devices') {
    console.log('Loading from endpoint:', endpoint);
    fetch(endpoint + (selectedGroups.length > 0 ? 
        '?' + selectedGroups.map(id => `driverGroupIds[]=${id}`).join('&') : ''))
        .then(response => response.json())
        .then(data => {
            if (data.error) throw new Error(data.error);
    
            console.log('Data received from backend:', data); // Add this debug line
            console.log('Number of items:', data.length);    // Add this debug line
            if (data.length > 0) {
                console.log('Sample item:', data[0]);        // Add this debug line
            }

            // Update groups filter section
            const groupsSection = document.getElementById('groupsFilterSection');
            groupsSection.innerHTML = `
                <div class="driver-groups-filter mb-3">
                    <label class="form-label d-flex justify-content-between align-items-center">
                        <span><i class="fas fa-users-rectangle me-2"></i>Filter by Driver Groups</span>
                        <button type="button" class="btn btn-sm btn-outline-secondary clear-groups">
                            <i class="fas fa-times me-1"></i>Clear
                        </button>
                    </label>
                    <div class="group-tags mb-2">
                        <div class="selected-groups" id="selectedGroups"></div>
                    </div>
                    <div class="dropdown w-100">
                        <input type="text" class="form-control" 
                               id="groupSearch" 
                               placeholder="Search and select groups..."
                               autocomplete="off"
                               data-bs-toggle="dropdown">
                        <ul class="dropdown-menu w-100" id="groupDropdown">
                            ${driverGroups.map(group => `
                                <li>
                                    <a class="dropdown-item" href="#" data-group-id="${group.id}">
                                        <div class="form-check">
                                            <input class="form-check-input" type="checkbox" 
                                                   value="${group.id}" 
                                                   id="group_${group.id}"
                                                   ${selectedGroups.includes(group.id) ? 'checked' : ''}>
                                            <label class="form-check-label" for="group_${group.id}">
                                                ${group.name}
                                            </label>
                                        </div>
                                    </a>
                                </li>
                            `).join('')}
                        </ul>
                    </div>
                </div>`;
    
            // Update devices section
            const deviceList = document.getElementById('deviceList');
            deviceList.innerHTML = `
                <div class="d-flex justify-content-between align-items-center mb-3">
                    <div class="btn-group">
                        <button type="button" class="btn btn-sm btn-outline-primary" id="selectAllDevices">
                            <i class="fas fa-check-square me-1"></i>Select All
                        </button>
                        <button type="button" class="btn btn-sm btn-outline-secondary" id="deselectAllDevices">
                            <i class="fas fa-square me-1"></i>Deselect All
                        </button>
                    </div>
                    <div class="input-group w-auto">
                        <span class="input-group-text">
                            <i class="fas fa-search"></i>
                        </span>
                        <input type="text" class="form-control" id="deviceSearch" 
                               placeholder="Search ${isDriverBased ? 'drivers' : 'assets'}...">
                    </div>
                </div>
                <div class="device-items">
                    ${data.map(item => `
                        <div class="device-item">
                            <div class="form-check">
                                <input class="form-check-input" type="checkbox" value="${item.id}" 
                                       id="device_${item.id}" name="devices">
                                <label class="form-check-label" for="device_${item.id}">
                                    ${item.name}
                                    ${!isDriverBased && item.serialNumber ? 
                                        `<span class="device-serial text-muted ms-2">${item.serialNumber}</span>` 
                                        : ''}
                                </label>
                            </div>
                        </div>
                    `).join('')}
                </div>`;
    
            // Setup group filtering
            const groupSearch = document.getElementById('groupSearch');
            const groupDropdown = document.getElementById('groupDropdown');
            const selectedGroupsContainer = document.getElementById('selectedGroups');
    
            // Update selected groups display
            function updateSelectedGroupsDisplay() {
                const selectedGroups = Array.from(document.querySelectorAll('#groupDropdown input:checked'))
                    .map(input => ({
                        id: input.value,
                        name: input.nextElementSibling.textContent.trim()
                    }));
    
                selectedGroupsContainer.innerHTML = selectedGroups.map(group => `
                    <span class="group-tag">
                        ${group.name}
                        <button type="button" class="btn-close btn-close-white btn-sm" 
                                data-group-id="${group.id}"></button>
                    </span>
                `).join('');
    
                // Update search input placeholder
                groupSearch.placeholder = selectedGroups.length > 0 ? 
                    "Add more groups..." : "Search and select groups...";
            }
    
            // Initialize selected groups display
            updateSelectedGroupsDisplay();
    
            // Group search functionality
            groupSearch.addEventListener('input', (e) => {
                const searchTerm = e.target.value.toLowerCase();
                document.querySelectorAll('#groupDropdown li').forEach(item => {
                    const text = item.textContent.toLowerCase();
                    item.style.display = text.includes(searchTerm) ? '' : 'none';
                });
            });
    
            // Fix for the checkboxes in dropdown
            document.querySelectorAll('#groupDropdown .dropdown-item').forEach(item => {
                item.addEventListener('click', (e) => {
                    e.preventDefault();
                    e.stopPropagation();
                    const checkbox = item.querySelector('input[type="checkbox"]');
                    checkbox.checked = !checkbox.checked;
                    
                    const selectedGroups = Array.from(document.querySelectorAll('#groupDropdown input:checked'))
                        .map(input => input.value);
                    
                    updateSelectedGroupsDisplay();
                    
                    // Important: reload devices with groups without changing the current HTML structure
                    fetch(endpoint + (selectedGroups.length > 0 ? 
                        '?' + selectedGroups.map(id => `driverGroupIds[]=${id}`).join('&') : ''))
                        .then(response => response.json())
                        .then(data => {
                            if (data.error) throw new Error(data.error);
                            
                            // Only update the device items part
                            const deviceItemsContainer = document.querySelector('.device-items');
                            deviceItemsContainer.innerHTML = data.map(item => `
                                <div class="device-item">
                                    <div class="form-check">
                                        <input class="form-check-input" type="checkbox" value="${item.id}" 
                                               id="device_${item.id}" name="devices">
                                        <label class="form-check-label" for="device_${item.id}">
                                            ${item.name}
                                            ${!isDriverBased && item.serialNumber ? 
                                                `<span class="device-serial">${item.serialNumber}</span>` 
                                                : ''}
                                        </label>
                                    </div>
                                </div>
                            `).join('');
                            
                            // Re-apply device handlers after updating the items
                            setupDeviceHandlers();
                        })
                        .catch(error => {
                            console.error('Error filtering devices:', error);
                        });
                });
            });
    
            // Handle remove group tag
            selectedGroupsContainer.addEventListener('click', (e) => {
                if (e.target.classList.contains('btn-close')) {
                    const groupId = e.target.dataset.groupId;
                    const checkbox = document.querySelector(`#group_${groupId}`);
                    if (checkbox) {
                        checkbox.checked = false;
                        const selectedGroups = Array.from(document.querySelectorAll('#groupDropdown input:checked'))
                            .map(input => input.value);
                        updateSelectedGroupsDisplay();
                        
                        // Same approach: reload devices without changing the whole structure
                        fetch(endpoint + (selectedGroups.length > 0 ? 
                            '?' + selectedGroups.map(id => `driverGroupIds[]=${id}`).join('&') : ''))
                            .then(response => response.json())
                            .then(data => {
                                if (data.error) throw new Error(data.error);
                                
                                const deviceItemsContainer = document.querySelector('.device-items');
                                deviceItemsContainer.innerHTML = data.map(item => `
                                    <div class="device-item">
                                        <div class="form-check">
                                            <input class="form-check-input" type="checkbox" value="${item.id}" 
                                                   id="device_${item.id}" name="devices">
                                            <label class="form-check-label" for="device_${item.id}">
                                                ${item.name}
                                                ${!isDriverBased && item.serialNumber ? 
                                                    `<span class="device-serial">${item.serialNumber}</span>` 
                                                    : ''}
                                            </label>
                                        </div>
                                    </div>
                                `).join('');
                                
                                setupDeviceHandlers();
                            })
                            .catch(error => {
                                console.error('Error filtering devices:', error);
                            });
                    }
                }
            });
    
            // Clear all groups
            document.querySelector('.clear-groups').addEventListener('click', () => {
                document.querySelectorAll('#groupDropdown input[type="checkbox"]')
                    .forEach(cb => cb.checked = false);
                updateSelectedGroupsDisplay();
                
                // Reload all devices
                fetch(endpoint)
                    .then(response => response.json())
                    .then(data => {
                        if (data.error) throw new Error(data.error);
                        
                        const deviceItemsContainer = document.querySelector('.device-items');
                        deviceItemsContainer.innerHTML = data.map(item => `
                            <div class="device-item">
                                <div class="form-check">
                                    <input class="form-check-input" type="checkbox" value="${item.id}" 
                                           id="device_${item.id}" name="devices">
                                    <label class="form-check-label" for="device_${item.id}">
                                        ${item.name}
                                        ${!isDriverBased && item.serialNumber ? 
                                            `<span class="device-serial">${item.serialNumber}</span>` 
                                            : ''}
                                    </label>
                                </div>
                            </div>
                        `).join('');
                        
                        setupDeviceHandlers();
                    })
                    .catch(error => {
                        console.error('Error loading all devices:', error);
                    });
            });
    
            // Add existing device handlers
            setupDeviceHandlers();
        })
        .catch(error => {
            const deviceList = document.getElementById('deviceList');
            deviceList.innerHTML = `
                <div class="alert alert-danger">
                    <i class="fas fa-exclamation-circle me-2"></i>
                    ${error.message}
                </div>`;
        });
}

    function showLoading(message) {
        loadingOverlay.style.display = 'flex';
        loadingMessage.textContent = message;
    }

    function hideLoading() {
        loadingOverlay.style.display = 'none';
    }

    function showStatus(message, isError = false) {
        statusMessage.innerHTML = `
            <div class="alert alert-${isError ? 'danger' : 'success'} alert-dismissible fade show" role="alert">
                <i class="fas fa-${isError ? 'exclamation-circle' : 'check-circle'} me-2"></i>
                ${message}
                <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
            </div>`;
    }

    // Update the rules loading section
    fetch('/api/rules')
        .then(response => response.json())
        .then(data => {
            if (data.error) {
                throw new Error(data.error);
            }

            const rules = Array.isArray(data) ? data : [];
            
            const ruleList = document.getElementById('ruleList');
            ruleList.innerHTML = `
                <div class="mb-3">
                    <div class="d-flex justify-content-between align-items-center">
                        <div class="btn-group">
                            <button type="button" class="btn btn-sm btn-outline-primary" id="selectAllRules">
                                <i class="fas fa-check-square me-1"></i>Select All
                            </button>
                            <button type="button" class="btn btn-sm btn-outline-secondary" id="deselectAllRules">
                                <i class="fas fa-square me-1"></i>Deselect All
                            </button>
                        </div>
                        <div class="input-group w-auto">
                            <span class="input-group-text">
                                <i class="fas fa-search"></i>
                            </span>
                            <input type="text" class="form-control" id="ruleSearch" 
                                placeholder="Search rules...">
                        </div>
                    </div>
                </div>
                <div class="rule-items">
                    ${rules.map(rule => `
                        <div class="rule-item">
                            <div class="form-check">
                                <input class="form-check-input" type="checkbox" value="${rule.id}" 
                                    id="rule_${rule.id}" name="rules">
                                <label class="form-check-label" for="rule_${rule.id}">
                                    ${rule.name}
                                    ${rule.description ? 
                                        `<small class="text-muted ms-2">${rule.description}</small>` 
                                        : ''}
                                </label>
                            </div>
                        </div>
                    `).join('')}
                </div>`;

            // Add search functionality
            const searchInput = document.getElementById('ruleSearch');
            searchInput.addEventListener('input', (e) => {
                const searchTerm = e.target.value.toLowerCase();
                document.querySelectorAll('.rule-item').forEach(item => {
                    const ruleText = item.textContent.toLowerCase();
                    item.style.display = ruleText.includes(searchTerm) ? '' : 'none';
                });
            });

            // Add rule selection handlers
            document.getElementById('selectAllRules').onclick = () => {
                const visibleRules = Array.from(document.querySelectorAll('.rule-item'))
                    .filter(item => item.style.display !== 'none');
                visibleRules.forEach(item => {
                    item.querySelector('input[type="checkbox"]').checked = true;
                });
            };

            document.getElementById('deselectAllRules').onclick = () => {
                const visibleRules = Array.from(document.querySelectorAll('.rule-item'))
                    .filter(item => item.style.display !== 'none');
                visibleRules.forEach(item => {
                    item.querySelector('input[type="checkbox"]').checked = false;
                });
            };
        })
        .catch(error => {
            const ruleList = document.getElementById('ruleList');
            ruleList.innerHTML = `
                <div class="alert alert-danger">
                    <i class="fas fa-exclamation-circle me-2"></i>
                    ${error.message}
                </div>`;
        });

// Add this function definition
function setupDeviceHandlers() {
    // Add search functionality
    const searchInput = document.getElementById('deviceSearch');
    searchInput.addEventListener('input', (e) => {
        const searchTerm = e.target.value.toLowerCase();
        document.querySelectorAll('.device-item').forEach(item => {
            const deviceText = item.textContent.toLowerCase();
            item.style.display = deviceText.includes(searchTerm) ? '' : 'none';
        });
    });

    // Add device selection handlers
    document.getElementById('selectAllDevices').onclick = () => {
        document.querySelectorAll('.device-item:not([style*="display: none"]) input[name="devices"]')
            .forEach(cb => cb.checked = true);
    };

    document.getElementById('deselectAllDevices').onclick = () => {
        document.querySelectorAll('input[name="devices"]')
            .forEach(cb => cb.checked = false);
    };

    // Add hover effect for device items
    document.querySelectorAll('.device-item').forEach(item => {
        item.addEventListener('mouseenter', () => {
            item.classList.add('hover');
        });
        
        item.addEventListener('mouseleave', () => {
            item.classList.remove('hover');
        });
    });
}

// Replace the columns loading section
fetch('/api/available-columns')
    .then(response => response.json())
    .then(data => {
        // Check if there's an error in the response
        if (data.error) {
            throw new Error(data.error);
        }

        // Ensure data is an object with arrays
        if (!data || typeof data !== 'object') {
            throw new Error('Invalid column data received');
        }

        const accordion = document.getElementById('columnAccordion');
        accordion.innerHTML = Object.entries(data).map(([group, fields]) => {
            if (!Array.isArray(fields)) {
                console.warn(`Invalid fields data for group ${group}`);
                return '';
            }

            return `
                <div class="accordion-item">
                    <h2 class="accordion-header">
                        <button class="accordion-button collapsed" type="button" 
                                data-bs-toggle="collapse" data-bs-target="#collapse${group}">
                            <i class="fas fa-${getGroupIcon(group)} me-2"></i>
                            ${formatGroupName(group)}
                            <span class="badge bg-primary ms-2" id="${group}Count">0</span>
                        </button>
                    </h2>
                    <div id="collapse${group}" class="accordion-collapse collapse"
                         data-bs-parent="#columnAccordion">
                        <div class="accordion-body">
                            <div class="mb-3">
                                <div class="btn-group">
                                    <button type="button" class="btn btn-sm btn-outline-primary select-all" 
                                            data-group="${group}">
                                        <i class="fas fa-check-square me-1"></i>Select All
                                    </button>
                                    <button type="button" class="btn btn-sm btn-outline-secondary deselect-all" 
                                            data-group="${group}">
                                        <i class="fas fa-square me-1"></i>Deselect All
                                    </button>
                                </div>
                            </div>
                            <div class="row">
                                ${fields.map(field => `
                                    <div class="col-md-4">
                                        <div class="form-check">
                                            <input class="form-check-input" type="checkbox" 
                                                   value="${field.source}.${field.id}" 
                                                   id="${field.source}_${field.id}" 
                                                   name="columns">
                                            <label class="form-check-label" for="${field.source}_${field.id}">
                                                ${field.name}
                                            </label>
                                        </div>
                                    </div>
                                `).join('')}
                            </div>
                        </div>
                    </div>
                </div>`;
        }).join('');

        // Add helper function for group icons
        function getGroupIcon(group) {
            const icons = {
                Rule: 'exclamation-triangle',
                Asset: 'truck',
                Driver: 'user'
            };
            return icons[group] || 'table';
        }

        // Add column selection handlers
        document.querySelectorAll('.select-all').forEach(btn => {
            btn.onclick = () => {
                const group = btn.dataset.group;
                document.querySelectorAll(`#collapse${group} input[type="checkbox"]`)
                    .forEach(cb => cb.checked = true);
                updateGroupCount(group);
                updateSelectedColumns(); // Add this line
            };
        });

        document.querySelectorAll('.deselect-all').forEach(btn => {
            btn.onclick = () => {
                const group = btn.dataset.group;
                document.querySelectorAll(`#collapse${group} input[type="checkbox"]`)
                    .forEach(cb => cb.checked = false);
                updateGroupCount(group);
                updateSelectedColumns(); // Add this line
            };
        });

        // Add change handlers for updating counts
        document.querySelectorAll('input[name="columns"]').forEach(checkbox => {
            checkbox.addEventListener('change', () => {
                const group = checkbox.closest('.accordion-item').querySelector('.accordion-button').dataset.bsTarget.replace('#collapse', '');
                updateGroupCount(group);
                updateSelectedColumns();
            });
        });

        function updateGroupCount(group) {
            const count = document.querySelectorAll(`#collapse${group} input[type="checkbox"]:checked`).length;
            document.getElementById(`${group}Count`).textContent = count;
        }
    })
    .catch(error => {
        const accordion = document.getElementById('columnAccordion');
        accordion.innerHTML = `
            <div class="alert alert-danger">
                <i class="fas fa-exclamation-circle me-2"></i>
                ${error.message}
            </div>`;
    });

    // Handle form submission
    reportForm.onsubmit = async (e) => {
        e.preventDefault();
        
        const selectedRules = Array.from(document.querySelectorAll('input[name="rules"]:checked'))
            .map(cb => cb.value);
        // Get columns in their displayed order
        const selectedColumns = Array.from(document.querySelectorAll('#selectedColumnsList .column-item'))
            .map(item => item.dataset.value);
        const selectedItems = Array.from(document.querySelectorAll('input[name="devices"]:checked'))
            .map(cb => cb.value);

        if (selectedRules.length === 0) {
            showStatus('Please select at least one rule type', true);
            return;
        }

        if (selectedColumns.length === 0) {
            showStatus('Please select at least one column', true);
            return;
        }

        const fromDate = document.getElementById('fromDate').value;
        const toDate = document.getElementById('toDate').value;
        const sortColumn = document.getElementById('sortColumn').value;

        showLoading('Generating report...');
        updateProgress('Starting report generation...', 0);

        try {
            const response = await fetch('/api/generate-report', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    fromDate: `${fromDate}T00:00:00.000Z`,
                    toDate: `${toDate}T23:59:59.999Z`,
                    ruleIds: selectedRules,
                    columns: selectedColumns,
                    deviceIds: selectedItems,
                    sortColumn: sortColumn,
                    sortAscending: sortAscending 
                })
            });

            if (response.ok) {
                checkReportStatus();
            } else {
                throw new Error('Failed to start report generation');
            }
        } catch (error) {
            hideLoading();
            showStatus(error.message, true);
        }
    };

    function checkReportStatus() {
        fetch('/api/report-status')
            .then(response => response.json())
            .then(data => {
                if (data.status === 'complete') {
                    updateProgress(data.message, 100);
                    setTimeout(() => {
                        hideLoading();
                        window.location.href = '/api/download-report';
                    }, 1000);
                } else if (data.status === 'error') {
                    throw new Error(data.message);
                } else {
                    updateProgress(data.message);
                    setTimeout(checkReportStatus, 1000);
                }
            })
            .catch(error => {
                hideLoading();
                showStatus(error.message, true);
            });
    }

    function updateProgress(message, percent) {
        loadingMessage.textContent = message;
        if (percent !== undefined) {
            progressBar.style.width = `${percent}%`;
        }
    }

    function formatGroupName(group) {
        return group.split('_')
            .map(word => word.charAt(0).toUpperCase() + word.slice(1))
            .join(' ');
    }
});

function updateSelectedColumns() {
    const selectedColumns = Array.from(document.querySelectorAll('input[name="columns"]:checked'));
    const selectedColumnsList = document.getElementById('selectedColumnsList');
    const noColumnsMessage = document.getElementById('noColumnsMessage');

    if (selectedColumns.length === 0) {
        selectedColumnsList.style.display = 'none';
        noColumnsMessage.style.display = 'block';
        return;
    }

    noColumnsMessage.style.display = 'none';
    selectedColumnsList.style.display = 'block';

    selectedColumnsList.innerHTML = selectedColumns.map((checkbox, index) => {
        const label = checkbox.nextElementSibling.textContent;
        const value = checkbox.value;
        
        return `
            <li class="column-item" data-value="${value}">
                <i class="fas fa-grip-vertical"></i>
                ${label}
                <span class="column-group">Column ${String.fromCharCode(65 + index)}</span>
            </li>`;
    }).join('');

    initDragAndDrop();

    // Update sort dropdown options
    const sortSelect = document.getElementById('sortColumn');
    const currentValue = sortSelect.value; // Store current selection
    
    // Create options from selected columns
    const options = ['<option value="">No sorting</option>'];
    selectedColumns.forEach(checkbox => {
        const label = checkbox.nextElementSibling.textContent;
        const value = checkbox.value;
        options.push(`<option value="${value}">${label}</option>`);
    });
    
    sortSelect.innerHTML = options.join('');
    sortSelect.value = currentValue; // Restore previous selection if still valid
}

function initDragAndDrop() {
    const columnsList = document.getElementById('selectedColumnsList');
    const items = columnsList.querySelectorAll('.column-item');
    let draggedItem = null;

    // Add function to update column letters
    function updateColumnLetters() {
        columnsList.querySelectorAll('.column-item').forEach((item, index) => {
            const letterSpan = item.querySelector('.column-group');
            letterSpan.textContent = `Column ${String.fromCharCode(65 + index)}`;
        });
    }

    items.forEach(item => {
        item.draggable = true;

        item.addEventListener('dragstart', (e) => {
            draggedItem = item;
            e.dataTransfer.effectAllowed = 'move';
            setTimeout(() => item.classList.add('dragging'), 0);
        });

        item.addEventListener('dragend', () => {
            item.classList.remove('dragging');
            items.forEach(item => item.classList.remove('drop-target'));
            draggedItem = null;
            // Update letters after drag ends
            updateColumnLetters();
        });

        item.addEventListener('dragover', (e) => {
            e.preventDefault();
            if (item === draggedItem) return;
            
            const bounding = item.getBoundingClientRect();
            const offset = bounding.y + (bounding.height / 2);
            
            if (e.clientY - offset > 0) {
                item.style.borderBottom = 'solid 2px var(--primary-color)';
                item.style.borderTop = '';
            } else {
                item.style.borderTop = 'solid 2px var(--primary-color)';
                item.style.borderBottom = '';
            }
        });

        item.addEventListener('dragleave', () => {
            item.style.borderTop = '';
            item.style.borderBottom = '';
        });

        item.addEventListener('drop', (e) => {
            e.preventDefault();
            if (item === draggedItem) return;

            const bounding = item.getBoundingClientRect();
            const offset = bounding.y + (bounding.height / 2);
            const insertAfter = e.clientY - offset > 0;

            item.style.borderTop = '';
            item.style.borderBottom = '';

            if (insertAfter) {
                item.parentNode.insertBefore(draggedItem, item.nextSibling);
            } else {
                item.parentNode.insertBefore(draggedItem, item);
            }

            // Update letters after the drop
            updateColumnLetters();
        });
    });
}

// Add this to your existing column checkbox change handlers
document.querySelectorAll('input[name="columns"]').forEach(checkbox => {
    checkbox.addEventListener('change', () => {
        const group = checkbox.closest('.accordion-item')
            .querySelector('.accordion-button').dataset.bsTarget.replace('#collapse', '');
        updateGroupCount(group);
        updateSelectedColumns();
    });
});

// Add this after your existing declarations
let sortAscending = true;

// Add this after your other event listeners
document.getElementById('sortDirection').addEventListener('click', function() {
    sortAscending = !sortAscending;
    const icon = this.querySelector('i');
    const text = sortAscending ? 'Ascending' : 'Descending';
    
    icon.className = sortAscending ? 
        'fas fa-sort-amount-down' : 
        'fas fa-sort-amount-up';
    this.innerHTML = `<i class="${icon.className}"></i> ${text}`;
});