/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\
*/

/*
██████╗ ██╗   ██╗████████╗████████╗ ████████╗ ███╗   ██╗
██╔══██╗██║   ██║╚══██╔══╝╚══██╔══╝██      ██╗████╗  ██║
██████╔╝██║   ██║   ██║      ██║   ██      ██║██╔██╗ ██║
██╔══██╗██║   ██║   ██║      ██║   ██      ██║██║╚██╗██║
██████╔╝╚██████╔╝   ██║      ██║    ████████╔╝██║ ╚████║
╚═════╝  ╚═════╝    ╚═╝      ╚═╝    ╚═══════╝ ╚═╝  ╚═══╝
*/

#ifndef INCLUDED_Button_h_
#define INCLUDED_Button_h_

class Button
{
	public:
	    int pin;
	    int io;
	    
	    // Button();
	    
	    // setup function
	    void setup(BelaContext *context, int pin_num, int io);
	    
	    void printStatus(BelaContext *context);
	    
	    // function to find the timing clicking once
	    bool pressDitection(BelaContext *context);
	private:
	    bool pressFlag = false;
};
    
#endif

/*
 ██████╗██╗  ██╗ █████╗ ███╗   ██╗███╗   ██╗ █████╗ ███████╗██╗     
██╔════╝██║  ██║██╔══██╗████╗  ██║████╗  ██║██╔══██╗██╔════╝██║     
██║     ███████║███████║██╔██╗ ██║██╔██╗ ██║███████║█████╗  ██║     
██║     ██╔══██║██╔══██║██║╚██╗██║██║╚██╗██║██╔══██║██╔══╝  ██║     
╚██████╗██║  ██║██║  ██║██║ ╚████║██║ ╚████║██║  ██║███████╗███████╗
 ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚══════╝
*/

#ifndef INCLUDED_Channel_h_
#define INCLUDED_Channel_h_
class Channel
{
	public:
		Channel();
		void volumeControl(bool buttonUp, bool buttonDown);
		bool setup(BelaContext *context, float vol, float max, float min, std::string* fileName, int fileNum);
		
		float amplitude;		// Amplitude of the sine wave (1.0 is maximum)
		float volume;
		float volumeMax;
		float volumeMin;
		int readPointer = 0;
		std::vector<std::vector<float>> sampleBuffer;
};


class AudioChannel:public Channel
{
	public:
		using Channel::Channel;
		bool setupAudio(BelaContext *context, float vol, float max, float min, std::string* fileName, int fileNum);
		void play(BelaContext *context, int channelFirst, int channelNum, bool startStop, int musicNum);
};

class AnalogChannel:public Channel
{
	public:
		using Channel::Channel;
		bool setupAnalog(BelaContext *context, float vol, float max, float min, std::string* fileName, int fileNum);
		void play(BelaContext *context, int channelFirst, int channelNum, bool startStop, int musicNum);
	private:
		float audioFramesPerAnalogFrame;
};

#endif

/*
 ██████╗███████╗██╗   ██╗
██╔════╝██╔════╝██║   ██║
██║     ███████╗██║   ██║
██║     ╚════██║╚██╗ ██╔╝
╚██████╗███████║ ╚████╔╝ 
 ╚═════╝╚══════╝  ╚═══╝  
*/
#ifndef INCLUDED_Csv_h_
#define INCLUDED_Csv_h_

class Csv {
	public:
		Csv();
	private:
		std::string filePath;
};

#endif


/*
██╗   ██╗ █████╗ ██╗      █████╗ ██████╗ ██╗     ███████╗███████╗
██║   ██║██╔══██╗██║     ██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
██║   ██║███████║██║     ███████║██████╔╝██║     █████╗  ███████╗
╚██╗ ██╔╝██╔══██║██║     ██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
 ╚████╔╝ ██║  ██║███████╗██║  ██║██████╔╝███████╗███████╗███████║
  ╚═══╝  ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝
*/

#ifndef INCLUDED_Valuables_h_
#define INCLUDED_Valuables_h_

// Pin number
extern int gButton0; // digital pin 0 - check the pin diagram in the IDE
extern int gButton1; // digital pin 1 - check the pin diagram in the IDE
extern int gButton2; // digital pin 2 - check the pin diagram in the IDE
extern int gLed3; // digital pin 3 - check the pin diagram in the IDE
extern int gButton4; // digital pin 4 - check the pin diagram in the IDE
extern int gButton5; // digital pin 5 - check the pin diagram in the IDE

// Music number and Volumes
extern int gMusicNum;
extern int gAudioVolume;
extern int gVibrationVolume;
extern int gOntennaVolume;

// Volume max and min
extern int gAudioVolumeMax;
extern int gAudioVolumeMin;
extern int gVibrationVolumeMax;
extern int gVibrationVolumeMin;
extern int gOntennaVolumeMax;
extern int gOntennaVolumeMin;

// Buttons
extern Button buttonVolumeSwitch;
extern Button buttonVolumeUp;
extern Button buttonVolumeDown;
extern Button buttonStartStop;
extern Button buttonMusicChange;

// Related to buttons
extern int gVolumeSwitch;
extern int gVolumeSwitchCoolTime;
extern int gPassedTime;
extern bool gStartStop;

// Channels
extern AudioChannel music;
extern AnalogChannel vibration;
extern AnalogChannel ontenna;

// Wavfiles
extern std::string drumFiles[];
extern std::string musicFiles[];

// Setting default values
extern std::string gFilePath;
extern std::ifstream gSetting;
extern std::string gLine;
extern std::vector<std::string> gLines;

#endif

/*
███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ████████╗ ███╗   ██╗███████╗
██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██      ██╗████╗  ██║██╔════╝
█████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██      ██║██╔██╗ ██║███████╗
██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██      ██║██║╚██╗██║╚════██║
██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║ ████████╔╝██║ ╚████║███████║
╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═══════╝ ╚═╝  ╚═══╝╚══════╝
*/

#ifndef INCLUDED_Functions_h_
#define INCLUDED_Functions_h_

bool settingRead();
void settingSave(std::string filePath, std::vector<std::string> labels, int musicNum, int audioVolume, int vibrationVolume, int ontennaVolume);
void pinRoll(BelaContext *context);

void checkButton(BelaContext *context);
void sendSignals(BelaContext *context);

float fromDb(float val, float min);
#endif
