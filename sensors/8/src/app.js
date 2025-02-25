window.addEventListener('DOMContentLoaded', (event) => {
  let audioContext, analyzer, microphone;

  const canvas = document.getElementById('visualizer');
  const canvasContext = canvas.getContext('2d');

  // Access the microphone
  async function getMicrophoneAccess() {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
      audioContext = new AudioContext();
      analyzer = audioContext.createAnalyser();
      microphone = audioContext.createMediaStreamSource(stream);
      microphone.connect(analyzer);
      analyzer.fftSize = 2048;
      draw();
    } catch (error) {
      console.error('Error accessing the microphone:', error);
    }
  }

  // Draw the audio data and change background based on volume
  function draw() {
    requestAnimationFrame(draw);
    const bufferLength = analyzer.frequencyBinCount;
    const dataArray = new Uint8Array(bufferLength);
    analyzer.getByteTimeDomainData(dataArray);
    
    // Calculate the average volume
    let sum = 0;
    for (let i = 0; i < bufferLength; i++) {
      sum += (dataArray[i] - 128) * (dataArray[i] - 128);
    }
    const averageVolume = sum / bufferLength;

    // Change the background color if volume is above a threshold
    if (averageVolume > 2000) { // You can adjust this threshold based on your needs
      canvas.style.backgroundColor = '#ff0000'; // Red background for high volume
    } else {
      canvas.style.backgroundColor = '#222'; // Normal background
    }

    canvasContext.clearRect(0, 0, canvas.width, canvas.height);
    const barWidth = (canvas.width / bufferLength) * 2.5;
    let barHeight;
    let x = 0;

    for (let i = 0; i < bufferLength; i++) {
      barHeight = dataArray[i];
      canvasContext.fillStyle = 'rgb(' + (barHeight+100) + ',50,50)';
      canvasContext.fillRect(x, canvas.height - barHeight / 2, barWidth, barHeight / 2);
      x += barWidth + 1;
    }
  }

  document.getElementById('start').addEventListener('click', getMicrophoneAccess);
});