/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

http://bela.io

C++ Real-Time Audio Programming with Bela - Lecture 2: Playing recorded samples
sample-player: partially finished example that plays a 
               recorded sound file in a loop.
*/

#include <Bela.h>
#include <libraries/AudioFile/AudioFile.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <other.h>


bool setup(BelaContext *context, void *userData)
{
	// setups
	bool setDefault = settingRead();
	bool setupMusic = music.setupAudio(context, gAudioVolume, gAudioVolumeMax, gAudioVolumeMin, musicFiles, 5);
	bool setupVibration = vibration.setupAnalog(context, gVibrationVolume, gVibrationVolumeMax, gVibrationVolumeMin, drumFiles, 5);
	bool setupOntenna = ontenna.setupAnalog(context, gOntennaVolume, gOntennaVolumeMax, gOntennaVolumeMin, drumFiles, 5);
	
	// Check if all setups is done properly
	if (setupMusic != true || setupVibration != true || setupOntenna != true || setDefault != true){
		return false;
	}
	
	// Pin roll Setting
	pinRoll(context);
	
	return true;
}

void render(BelaContext *context, void *userData)
{
	checkButton(context);
	sendSignals(context);
    
}

void cleanup(BelaContext *context, void *userData)
{
	settingSave(gFilePath, gLines, gMusicNum, music.volume, vibration.volume, ontenna.volume);
}
