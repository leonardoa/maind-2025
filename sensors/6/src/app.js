document.addEventListener('DOMContentLoaded', function() {
  const compassImage = document.getElementById('compassImage');
  const compassData = document.getElementById('compassData');

  // Function to determine cardinal direction
  function getCardinalDirection(angle) {
      const directions = ['North', 'North-East', 'East', 'South-East', 'South', 'South-West', 'West', 'North-West'];
      return directions[Math.round(((angle %= 360) < 0 ? angle + 360 : angle) / 45) % 8];
  }

  function updateCompass(event) {
      if (event.alpha !== null) {
          const alpha = event.alpha;
          // Rotate the compass needle
          compassImage.style.transform = `rotate(${-alpha}deg)`;
          // Get the cardinal direction
          const cardinalDirection = getCardinalDirection(alpha);
          compassData.innerHTML = `Magnetic Heading: ${alpha.toFixed(2)}° ->${cardinalDirection}`;
      }
  }

  // Check if DeviceOrientationEvent is supported
  if ('DeviceOrientationEvent' in window) {
      // Request permission for iOS devices
      if (DeviceMotionEvent && typeof DeviceMotionEvent.requestPermission === 'function') {
          DeviceMotionEvent.requestPermission()
              .then(permissionState => {
                  if (permissionState === 'granted') {
                      window.addEventListener('deviceorientation', updateCompass);
                  } else {
                      compassData.innerHTML = "Permission to access sensor was denied.";
                  }
              })
              .catch(console.error);
      } else {
          window.addEventListener('deviceorientation', updateCompass);
      }
  } else {
      compassData.innerHTML = "Your device does not support device orientation.";
  }
});
