document.addEventListener('DOMContentLoaded', function() {
  const compassImage = document.getElementById('compassImage');
  const compassData = document.getElementById('compassData');

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

  function updateCompass(event) {
      if (event.alpha !== null) {
          const alpha = event.alpha;
          // Rotate the compass needle
          compassImage.style.transform = `rotate(${-alpha}deg)`;
          compassData.innerHTML = `Magnetic Heading: ${alpha.toFixed(2)}°`;
      }
  }
});
