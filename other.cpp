/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\
*/

// Functions.cpp: a cpp file for functions

#include <Bela.h>
#include <libraries/AudioFile/AudioFile.h>
#include <iostream>
#include <math.h>
#include <other.h>
#include <sstream>
#include <fstream>


/*
██████╗ ██╗   ██╗████████╗████████╗ ████████╗ ███╗   ██╗
██╔══██╗██║   ██║╚══██╔══╝╚══██╔══╝██      ██ ████╗  ██║
██████╔╝██║   ██║   ██║      ██║   ██      ██ ██╔██╗ ██║
██╔══██╗██║   ██║   ██║      ██║   ██      ██ ██║╚██╗██║
██████╔╝╚██████╔╝   ██║      ██║    ████████║ ██║ ╚████║
╚═════╝  ╚═════╝    ╚═╝      ╚═╝    ╚═══════╝ ╚═╝  ╚═══╝
*/

// setup function
void Button::setup(BelaContext *context, int pin_num, int io){
	pin = pin_num;
	pinMode(context, 0, pin_num, io); //setting pin number and I/O
};

// void Button::printStatus(BelaContext *context){
// 	for (int i = 0; i < context->audioFrames; i++){
// 		// bool status = digitalRead(context, i, pin);
// 		// rt_printf("%d", status);
// 	}
// }

// function to find the timing clicking once
bool Button::pressDitection(BelaContext *context){
	// Check all frames
	for (int frame = 0; frame < context->audioFrames; frame++){
		// Read pin status
		bool status = digitalRead(context, frame, pin);
		
		// retur true at the only frame pressing button and released in the previous frame
		if (status == HIGH){
			pressFlag = false;
		}else if(status == LOW && pressFlag == false){
			pressFlag = true;
			return true;
		}
	}
	return false;
};

/*
 ██████╗██╗  ██╗ █████╗ ███╗   ██╗███╗   ██╗ █████╗ ███████╗██╗     
██╔════╝██║  ██║██╔══██╗████╗  ██║████╗  ██║██╔══██╗██╔════╝██║     
██║     ███████║███████║██╔██╗ ██║██╔██╗ ██║███████║█████╗  ██║     
██║     ██╔══██║██╔══██║██║╚██╗██║██║╚██╗██║██╔══██║██╔══╝  ██║     
╚██████╗██║  ██║██║  ██║██║ ╚████║██║ ╚████║██║  ██║███████╗███████╗
 ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚══════╝
*/

Channel::Channel(){};

void Channel::volumeControl(bool buttonUp, bool buttonDown){
	if (buttonUp == true && volume < volumeMax - 0.1){
		volume += 5; // unit is DB
		// rt_printf("%d", volume);
	}
	if (buttonDown == true && volume > volumeMin + 0.1){
		volume -= 5;
		// rt_printf("%d", volume);
	}
	
	
	// Convert from DB to amplitude
	amplitude = fromDb(volume, volumeMin);
};

bool Channel::setup(BelaContext *context, float vol, float max, float min, std::string* fileName, int fileNum){
	volume = vol;
	volumeMax = max;
	volumeMin = min;
	amplitude = fromDb(volume, volumeMin);
	
	// read wav files
	for (int musicNum = 0; musicNum < fileNum; musicNum++){
		sampleBuffer.push_back(AudioFileUtilities::loadMono(fileName[musicNum]));
		
		rt_printf("Music Number: %d ", musicNum);
		
		if(sampleBuffer[musicNum].size() == 0) {
	    	rt_printf("Error loading a file for audio channels '%s'\n", fileName[musicNum].c_str());
	    	return false;
		}

		rt_printf("Loaded the a file for audio channels '%s' with %d frames (%.1f seconds)\n", 
			fileName[musicNum].c_str(), sampleBuffer[musicNum].size(), // the number of the samples
			sampleBuffer[musicNum].size() / context->audioSampleRate); // the duration of the sound based on the audioSampleRate
	}
	
	return true;
}

// AudioChannel

bool AudioChannel::setupAudio(BelaContext *context, float vol, float max, float min, std::string* fileName, int fileNum){
	
	
	// if(sampleBuffer.size() == 0) {
 //   	rt_printf("Error loading a file for audio channels '%s'\n", fileName.c_str());
 //   	return false;
	// }

	// rt_printf("Loaded the a file for audio channels '%s' with %d frames (%.1f seconds)\n", 
	// 	fileName.c_str(), sampleBuffer.size(), // the number of the samples
	// 	sampleBuffer.size() / context->audioSampleRate); // the duration of the sound based on the audioSampleRate
	
	return setup(context, vol, max, min, fileName, fileNum);
};

void AudioChannel::play(BelaContext *context, int channelFirst, int channelNum, bool startStop, int musicNum){
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		float out = amplitude* sampleBuffer[musicNum][readPointer];
		
		for(unsigned int channel = channelFirst; channel < channelNum; channel++) {
			if (startStop == true){
				audioWrite(context, n, channel, out);
			}else {
				audioWrite(context, n, channel, 0);
			}
	    }
	    
	    if (startStop == true){
	    	readPointer++;
	    }
	        
	    if(readPointer >= sampleBuffer[musicNum].size()){
	    	readPointer = 0;
	    }
		
	}
};

//AnalogChannel

bool AnalogChannel::setupAnalog(BelaContext *context, float vol, float max, float min, std::string* fileName, int fileNum){
	bool test = setup(context, vol, max, min, fileName, fileNum);
	
	if (test == false){
		return false;
	}
	
	// if(sampleBuffer.size() == 0) {
 //   	rt_printf("Error loading a file for analog channels '%s'\n", fileName.c_str());
 //   	return false;
	// }

	// rt_printf("Loaded the a file for analog channels '%s' with %d frames (%.1f seconds)\n", 
	// 	fileName.c_str(), sampleBuffer.size(), // the number of the samples
	// 	sampleBuffer.size() / context->audioSampleRate); // the duration of the sound based on the audioSampleRate

	
	// Check if sample rate is as expected
	if((context->audioSampleRate == context->analogSampleRate) || (context->audioSampleRate == 2* context->analogSampleRate)) // when using 4 or 8 analog channels
	{
		audioFramesPerAnalogFrame =  context->audioSampleRate / context->analogSampleRate;
	}else
	{
		rt_printf("The ratio of the sample rates are unexpected \n audio sample rate: %.1f\n analog sample rate: %.1f \n Please use 4 or 8 analog channels", context->audioSampleRate, context->analogSampleRate);
        return false;
	} // Just in case the sample rate is weird
	
	return true;
};

void AnalogChannel::play(BelaContext *context, int channelFirst, int channelNum, bool startStop, int musicNum){
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		float out = amplitude* sampleBuffer[musicNum][readPointer/audioFramesPerAnalogFrame];
		
		for(unsigned int channel = channelFirst; channel < channelFirst + channelNum; channel++) {
			if (startStop == true){
				analogWriteOnce(context, n, channel, out);
			}else {
			analogWriteOnce(context, n, channel, 0);
			}
	    }
	    
	    if (startStop == true){
	    	readPointer++;
	    }
	        
	    if(readPointer >= sampleBuffer[musicNum].size()){
	    	readPointer = 0;
	    }
	}
};

/*
██╗   ██╗ █████╗ ██╗      █████╗ ██████╗ ██╗     ███████╗███████╗
██║   ██║██╔══██╗██║     ██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
██║   ██║███████║██║     ███████║██████╔╝██║     █████╗  ███████╗
╚██╗ ██╔╝██╔══██║██║     ██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
 ╚████╔╝ ██║  ██║███████╗██║  ██║██████╔╝███████╗███████╗███████║
  ╚═══╝  ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝
*/

int gButton0 = 0; // digital pin 0 - check the pin diagram in the IDE
int gButton1 = 1; // digital pin 1 - check the pin diagram in the IDE
int gButton2 = 2; // digital pin 2 - check the pin diagram in the IDE
int gLed3 = 3; // digital pin 3 - check the pin diagram in the IDE
int gButton4 = 4; // digital pin 4 - check the pin diagram in the IDE
int gButton5 = 5; // digital pin 5 - check the pin diagram in the IDE

int gMusicNum;
int gAudioVolume;
int gVibrationVolume;
int gOntennaVolume;

int gAudioVolumeMax = -20;
int gAudioVolumeMin = -60;
int gVibrationVolumeMax = 0;
int gVibrationVolumeMin = -40;
int gOntennaVolumeMax = 0;
int gOntennaVolumeMin = -40;

int gVolumeSwitch = 0;
int gVolumeSwitchCoolTime;
int gPassedTime = 0;
bool gStartStop = true;

Button buttonVolumeSwitch;
Button buttonVolumeUp;
Button buttonVolumeDown;
Button buttonStartStop;
Button buttonMusicChange;

AudioChannel music;
AnalogChannel vibration;
AnalogChannel ontenna;

std::string drumFiles[] = {
	"BillieJeanDrum.wav", 
	"SuavementeDrum.wav", 
	"RainDrum.wav",
	"HeavyRotationDrum.wav",
	"ImagineDrum.wav"
};

std::string musicFiles[] = {
	"BillieJeanMusic.wav", 
	"SuavementeMusic.wav", 
	"RainMusic.wav",
	"HeavyRotationMusic.wav",
	"ImagineMusic.wav"
};

std::string gFilePath = "setting.csv";
std::ifstream gSetting(gFilePath);
std::string gLine;
std::vector<std::string> gLines;


/*
███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ████████╗ ███╗   ██╗███████╗
██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██      ██╗████╗  ██║██╔════╝
█████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██      ██║██╔██╗ ██║███████╗
██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██      ██║██║╚██╗██║╚════██║
██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║ ████████╔╝██║ ╚████║███████║
╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═══════╝ ╚═╝  ╚═══╝╚══════╝
*/


bool settingRead(){
	while (getline(gSetting, gLine)) {
        gLines.push_back(gLine);
    }
    gSetting.close();

    if (gLines.size() < 2) {
        rt_printf("No enough lines in %s\n", gFilePath.c_str());
        return false;
    }
    
    std::stringstream ss(gLines[1]);
    std::vector<std::string> data;
    std::string value;
    while (getline(ss, value, ',')) {
        data.push_back(value);
    }
    
    if (data.size() >= 4) {
        gMusicNum = std::stoi(data[0]); // Music_Number
        gAudioVolume = std::stoi(data[1]); // Audio_Volume
        rt_printf("%d", gAudioVolume);
        gVibrationVolume = std::stoi(data[2]); // Vibration_Volume
        gOntennaVolume = std::stoi(data[3]); // Ontenna_Volume
    } else {
        rt_printf("No enough data in %s\n", gFilePath.c_str());
        return false;
    }
    
    return true;
};

void pinRoll(BelaContext *context){
	gVolumeSwitchCoolTime = context->audioSampleRate / context->audioFrames / 2;
	
	buttonVolumeUp.setup(context, gButton0, INPUT);
	buttonVolumeDown.setup(context, gButton1, INPUT);
	buttonVolumeSwitch.setup(context, gButton2, INPUT);
	buttonStartStop.setup(context, gButton4, INPUT);
	buttonMusicChange.setup(context, gButton5, INPUT);
	pinMode(context, 0, gLed3, OUTPUT); 
};

void checkButton(BelaContext *context){
	// Volume switch 
	// gVolumeSwitch
	// 0: Music from headphone
	// 1: Drum from actuaters
	// 2: Drum from ontenna
	if (buttonVolumeSwitch.pressDitection(context) == true){
		gVolumeSwitch += 1;
		gVolumeSwitch %= 3;
	}
	
	// Music Change
	if (buttonMusicChange.pressDitection(context) == true){
		gMusicNum += 1;
		gMusicNum %= 5;
		music.readPointer = 0;
		vibration.readPointer = 0;
	}
	
	
	// volume control
	bool volumeUp = buttonVolumeUp.pressDitection(context);
	bool volumeDown = buttonVolumeDown.pressDitection(context);
	
	if (gVolumeSwitch == 0){
    	music.volumeControl(volumeUp, volumeDown);
    	for(unsigned int n = 0; n < context->audioFrames; n++){
    		digitalWriteOnce(context, n, gLed3, HIGH); // LED on
		}
    }else if (gVolumeSwitch == 1){
    	vibration.volumeControl(volumeUp, volumeDown);
    	if (gPassedTime < gVolumeSwitchCoolTime){
    		for(unsigned int n = 0; n < context->audioFrames; n++){
    			digitalWriteOnce(context, n, gLed3, HIGH); // LED blink
			}
    	}else if(gPassedTime < 2*gVolumeSwitchCoolTime){
    		for(unsigned int n = 0; n < context->audioFrames; n++){
    			digitalWriteOnce(context, n, gLed3, LOW);// LED blink
			}	
    	}else{
    		gPassedTime = 0;
    	}
    	gPassedTime += 1;
    }else {
    	ontenna.volumeControl(volumeUp, volumeDown);
    	for(unsigned int n = 0; n < context->audioFrames; n++){
    		digitalWriteOnce(context, n, gLed3, LOW); // LED off
		}
    }
	
	if (buttonStartStop.pressDitection(context) == true){
		gStartStop = !gStartStop;
	}
};

void sendSignals(BelaContext *context){
	music.play(context, 0, 2, gStartStop, gMusicNum);
	vibration.play(context, 0, 2, gStartStop, gMusicNum);
	ontenna.play(context, 2, 2, gStartStop, gMusicNum);	
};

void settingSave(std::string filePath, std::vector<std::string> labels, int musicNum, int audioVolume, int vibrationVolume, int ontennaVolume){
	std::stringstream newData;
    newData 
    << musicNum << ","
    << audioVolume << ","
    << vibrationVolume << ","
    << ontennaVolume; // 
    
    std::ofstream outFile(filePath);
    outFile << labels[0] << std::endl; // save labes
    outFile << newData.str() << std::endl; // save setting values
};


float fromDb (float val, float min){
	if (val < min){
		return 0;
	}
	
	return powf(10.0, val/20);
};