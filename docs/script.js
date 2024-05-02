const audioContext = new AudioContext();
let source = null;
let leftGainNode = audioContext.createGain();
let rightGainNode = audioContext.createGain();
let splitter = audioContext.createChannelSplitter(2);
let merger = audioContext.createChannelMerger(2);

async function loadAudio() {
  const response = await fetch('test.wav');
  const arrayBuffer = await response.arrayBuffer();
  const audioBuffer = await audioContext.decodeAudioData(arrayBuffer);
  source = audioContext.createBufferSource();
  source.buffer = audioBuffer;
  source.loop = true;

  source.connect(splitter);
  splitter.connect(leftGainNode, 0);
  splitter.connect(rightGainNode, 1);
  leftGainNode.connect(merger, 0, 0);
  rightGainNode.connect(merger, 0, 1);
  merger.connect(audioContext.destination);
}

function playAudio() {
  if (!source) {
    loadAudio().then(() => source.start(0));
  } else if (audioContext.state === 'suspended') {
    audioContext.resume();
  } else {
    source.start(0);
  }
}

function stopAudio() {
  if (source) {
    source.stop();
    source = null;
  }
}

function adjustVolume(value, channel) {
  const volume = parseFloat(value);
  if (channel === 'left') {
    leftGainNode.gain.value = volume;
  } else if (channel === 'right') {
    rightGainNode.gain.value = volume;
  }
}

// 加速度センサーからデータを取得し、表示する関数
// 加速度センサーからデータを取得し、表示する関数
function setupAccelerometer() {
  if (window.DeviceMotionEvent) {
    window.addEventListener('devicemotion', (event) => {
      // 重力を含む加速度データ
      document.getElementById('accelX').textContent = event.accelerationIncludingGravity.x.toFixed(2);
      document.getElementById('accelY').textContent = event.accelerationIncludingGravity.y.toFixed(2);
      document.getElementById('accelZ').textContent = event.accelerationIncludingGravity.z.toFixed(2);
      
      // 重力を含まない加速度データ
      if (event.acceleration) {
        document.getElementById('accelXNoGrav').textContent = event.acceleration.x.toFixed(2);
        document.getElementById('accelYNoGrav').textContent = event.acceleration.y.toFixed(2);
        document.getElementById('accelZNoGrav').textContent = event.acceleration.z.toFixed(2);
      } else {
        document.getElementById('accelerometerNoGravity').innerHTML = "<p>Non-gravitational accelerometer not supported.</p>";
      }
    }, true);
  } else {
    document.getElementById('accelerometer').innerHTML = "<p>Accelerometer not supported.</p>";
    document.getElementById('accelerometerNoGravity').innerHTML = "<p>Non-gravitational accelerometer not supported.</p>";
  }
}

// ページが完全に読み込まれたときに加速度センサーをセットアップ
document.addEventListener('DOMContentLoaded', setupAccelerometer);


