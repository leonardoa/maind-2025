document.addEventListener('DOMContentLoaded', function() {
    const compassImage = document.getElementById('compassImage');
    const compassData = document.getElementById('compassData');
    const requestPermissionBtn = document.getElementById('requestPermission');

    // Function to determine cardinal direction
    function getCardinalDirection(angle) {
        const directions = ['North', 'North-East', 'East', 'South-East', 'South', 'South-West', 'West', 'North-West'];
        angle += 22.5; // Adjust so the split between directions is centered on the cardinal direction
        if (angle < 0) angle += 360;
        angle %= 360;
        return directions[Math.floor(angle / 45)];
    }

    // Adjust the alpha angle for magnetic declination
    const magneticDeclination = 0; // Set your location's magnetic declination in degrees here

    function updateCompass(event) {
        if (event.alpha !== null) {
            let alpha = event.alpha;
            alpha += magneticDeclination; // Adjust compass reading by adding the magnetic declination

            // Rotate the compass needle
            compassImage.style.transform = `rotate(${-alpha}deg)`;

            // Get the cardinal direction
            const cardinalDirection = getCardinalDirection(alpha);
            compassData.innerHTML = `Magnetic Heading: ${alpha.toFixed(2)}° ${cardinalDirection}`;
        }
    }

    function enableCompass() {
        if ('DeviceOrientationEvent' in window && typeof DeviceOrientationEvent.requestPermission === 'function') {
            // Request permission for iOS devices
            DeviceOrientationEvent.requestPermission()
                .then(permissionState => {
                    if (permissionState === 'granted') {
                        window.addEventListener('deviceorientation', updateCompass);
                        requestPermissionBtn.style.display = 'none'; // Hide button after granting permission
                    } else {
                        compassData.innerHTML = "Permission to access sensor was denied.";
                    }
                })
                .catch(error => {
                    console.error(error);
                    compassData.innerHTML = "Error requesting permission.";
                });
        } else {
            // Directly add event listener for non-iOS devices
            window.addEventListener('deviceorientation', updateCompass);
            requestPermissionBtn.style.display = 'none'; // Hide button if not needed
        }
    }

    // Check if permission request is needed
    if ('DeviceOrientationEvent' in window && typeof DeviceOrientationEvent.requestPermission === 'function') {
        requestPermissionBtn.style.display = 'block';
        requestPermissionBtn.addEventListener('click', enableCompass);
    } else {
        enableCompass(); // Enable immediately if permission is not required
    }
});