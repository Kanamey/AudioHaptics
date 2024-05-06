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

function consoleTest() {
  console.log("finally!");
}

function adjustVolume(value, channel) {
  const volume = parseFloat(value);
  if (channel === 'left') {
    leftGainNode.gain.value = volume;
  } else if (channel === 'right') {
    rightGainNode.gain.value = volume;
  }
}

function handleOrientation(event) {
  updateFieldIfNotNull('Orientation_a', event.alpha);
  updateFieldIfNotNull('Orientation_b', event.beta);
  updateFieldIfNotNull('Orientation_g', event.gamma);
  incrementEventCount();
}

function incrementEventCount(){
  let counterElement = document.getElementById("num-observed-events")
  let eventCount = parseInt(counterElement.innerHTML)
  counterElement.innerHTML = eventCount + 1;
}

function updateFieldIfNotNull(fieldName, value, precision=10){
  if (value != null)
    document.getElementById(fieldName).innerHTML = value.toFixed(precision);
}

function handleMotion(event) {
  updateFieldIfNotNull('Accelerometer_gx', event.accelerationIncludingGravity.x);
  updateFieldIfNotNull('Accelerometer_gy', event.accelerationIncludingGravity.y);
  updateFieldIfNotNull('Accelerometer_gz', event.accelerationIncludingGravity.z);

  updateFieldIfNotNull('Accelerometer_x', event.acceleration.x);
  updateFieldIfNotNull('Accelerometer_y', event.acceleration.y);
  updateFieldIfNotNull('Accelerometer_z', event.acceleration.z);

  updateFieldIfNotNull('Accelerometer_i', event.interval, 2);

  updateFieldIfNotNull('Gyroscope_z', event.rotationRate.alpha);
  updateFieldIfNotNull('Gyroscope_x', event.rotationRate.beta);
  updateFieldIfNotNull('Gyroscope_y', event.rotationRate.gamma);
  incrementEventCount();
}

let is_running = false;
let demo_button = document.getElementById("start_demo");
demo_button.onclick = function(e) {
  e.preventDefault();
  
  // Request permission for iOS 13+ devices
  if (
    DeviceMotionEvent &&
    typeof DeviceMotionEvent.requestPermission === "function"
  ) {
    DeviceMotionEvent.requestPermission();
  }
  
  if (is_running){
    window.removeEventListener("devicemotion", handleMotion);
    window.removeEventListener("deviceorientation", handleOrientation);
    demo_button.innerHTML = "Start demo";
    demo_button.classList.add('btn-success');
    demo_button.classList.remove('btn-danger');
    is_running = false;
  }else{
    window.addEventListener("devicemotion", handleMotion);
    window.addEventListener("deviceorientation", handleOrientation);
    document.getElementById("start_demo").innerHTML = "Stop demo";
    demo_button.classList.remove('btn-success');
    demo_button.classList.add('btn-danger');
    is_running = true;
  }
};

/*
Light and proximity are not supported anymore by mainstream browsers.
window.addEventListener('devicelight', function(e) {
   document.getElementById("DeviceLight").innerHTML="AmbientLight current Value: "+e.value+" Max: "+e.max+" Min: "+e.min;
});

window.addEventListener('lightlevel', function(e) {
   document.getElementById("Lightlevel").innerHTML="Light level: "+e.value;
});

window.addEventListener('deviceproximity', function(e) {
   document.getElementById("DeviceProximity").innerHTML="DeviceProximity current Value: "+e.value+" Max: "+e.max+" Min: "+e.min;
});

window.addEventListener('userproximity', function(event) {
   document.getElementById("UserProximity").innerHTML="UserProximity: "+event.near;
});
*/


// Get Acceleration
// function setupAccelerometer() {
//   if (window.DeviceMotionEvent) {
//     window.addEventListener('devicemotion', (event) => {
//       // Acceleration including gravity
//       updateFieldIfNotNull('Accelerometer_gx', event.accelerationIncludingGravity.x);
//       updateFieldIfNotNull('Accelerometer_gy', event.accelerationIncludingGravity.y);
//       updateFieldIfNotNull('Accelerometer_gz', event.accelerationIncludingGravity.z);

//       // Acceleration without gravity
//       if (event.acceleration) {
//         updateFieldIfNotNull('Accelerometer_x', event.acceleration.x);
//         updateFieldIfNotNull('Accelerometer_y', event.acceleration.y);
//         updateFieldIfNotNull('Accelerometer_z', event.acceleration.z);
//       } else {
//         document.getElementById('accelerometerNoGravity').innerHTML = "<p>Non-gravitational accelerometer not supported.</p>";
//       }

//       // Orientation (alpha, beta, gamma)
//       handleOrientation(event);

//       // Gyroscope data
//       updateFieldIfNotNull('Gyroscope_x', event.rotationRate.beta);
//       updateFieldIfNotNull('Gyroscope_y', event.rotationRate.gamma);
//       updateFieldIfNotNull('Gyroscope_z', event.rotationRate.alpha);

//       // Data interval
//       updateFieldIfNotNull('Accelerometer_i', event.interval, 2);

//       incrementEventCount();
//     }, true);
//   } else {
//     document.getElementById('accelerometer').innerHTML = "<p>Accelerometer not supported.</p>";
//     document.getElementById('accelerometerNoGravity').innerHTML = "<p>Non-gravitational accelerometer not supported.</p>";
//   }
// }

// // ページが完全に読み込まれたときに加速度センサーをセットアップ
// document.addEventListener('DOMContentLoaded', setupAccelerometer);



// // Get Acceleration
// function setupAccelerometer() {
//   if (window.DeviceMotionEvent) {
//     window.addEventListener('devicemotion', (event) => {
//       // Acceleration
//       document.getElementById('accelX').textContent = event.accelerationIncludingGravity.x.toFixed(2);
//       document.getElementById('accelY').textContent = event.accelerationIncludingGravity.y.toFixed(2);
//       document.getElementById('accelZ').textContent = event.accelerationIncludingGravity.z.toFixed(2);
      
//       // Acceleration
//       if (event.acceleration) {
//         document.getElementById('accelXNoGrav').textContent = event.acceleration.x.toFixed(2);
//         document.getElementById('accelYNoGrav').textContent = event.acceleration.y.toFixed(2);
//         document.getElementById('accelZNoGrav').textContent = event.acceleration.z.toFixed(2);
//       } else {
//         document.getElementById('accelerometerNoGravity').innerHTML = "<p>Non-gravitational accelerometer not supported.</p>";
//       }
//     }, true);
//   } else {
//     document.getElementById('accelerometer').innerHTML = "<p>Accelerometer not supported.</p>";
//     document.getElementById('accelerometerNoGravity').innerHTML = "<p>Non-gravitational accelerometer not supported.</p>";
//   }
// }

// function handleOrientation(event) {
//   updateFieldIfNotNull('Orientation_a', event.alpha);
//   updateFieldIfNotNull('Orientation_b', event.beta);
//   updateFieldIfNotNull('Orientation_g', event.gamma);
//   incrementEventCount();
// }

// function handleOrientation(event) {
//   updateFieldIfNotNull('Orientation_a', event.alpha);
//   updateFieldIfNotNull('Orientation_b', event.beta);
//   updateFieldIfNotNull('Orientation_g', event.gamma);
//   incrementEventCount();
// }

// function incrementEventCount(){
//   let counterElement = document.getElementById("num-observed-events")
//   let eventCount = parseInt(counterElement.innerHTML)
//   counterElement.innerHTML = eventCount + 1;
// }

// function updateFieldIfNotNull(fieldName, value, precision=10){
//   if (value != null)
//     document.getElementById(fieldName).innerHTML = value.toFixed(precision);
// }

// function handleMotion(event) {
//   updateFieldIfNotNull('Accelerometer_gx', event.accelerationIncludingGravity.x);
//   updateFieldIfNotNull('Accelerometer_gy', event.accelerationIncludingGravity.y);
//   updateFieldIfNotNull('Accelerometer_gz', event.accelerationIncludingGravity.z);

//   updateFieldIfNotNull('Accelerometer_x', event.acceleration.x);
//   updateFieldIfNotNull('Accelerometer_y', event.acceleration.y);
//   updateFieldIfNotNull('Accelerometer_z', event.acceleration.z);

//   updateFieldIfNotNull('Accelerometer_i', event.interval, 2);

//   updateFieldIfNotNull('Gyroscope_z', event.rotationRate.alpha);
//   updateFieldIfNotNull('Gyroscope_x', event.rotationRate.beta);
//   updateFieldIfNotNull('Gyroscope_y', event.rotationRate.gamma);
//   incrementEventCount();
// }

