window.addEventListener('DOMContentLoaded', (event) => {
  let audioContext, analyzer, microphone;

  // Access the microphone
  async function getMicrophoneAccess() {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
      audioContext = new AudioContext();
      analyzer = audioContext.createAnalyser();
      microphone = audioContext.createMediaStreamSource(stream);
      microphone.connect(analyzer);
      analyzer.fftSize = 2048;
      updateVolume();
    } catch (error) {
      console.error('Error accessing the microphone:', error);
      document.getElementById("volume").textContent = "Error accessing the microphone.";
    }
  }

  // Update the displayed volume
  function updateVolume() {
    requestAnimationFrame(updateVolume);
    const bufferLength = analyzer.fftSize;
    const dataArray = new Uint8Array(bufferLength);
    analyzer.getByteTimeDomainData(dataArray);
    let sum = 0;
    for (let i = 0; i < bufferLength; i++) {
      let value = dataArray[i] - 128; // Centering the value around 0
      sum += value * value;
    }
    const average = Math.sqrt(sum / bufferLength);
    document.getElementById("volume").textContent = "Current Volume: " + average.toFixed(2);
    if (average > 2) {
      document.body.style.backgroundColor = "red";
    }
    else {
      document.body.style.backgroundColor = "white";
    }
  }

  document.getElementById('start').addEventListener('click', getMicrophoneAccess);
});