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


// BillieJean 
std::string gFilenameMusic = "BillieJeanMusic.wav";	// Name of the sound file (in project folder)
std::string gFilenameDrum = "BillieJeanDrum.wav";	// Name of the sound file (in project folder)

// Suavemente 
// std::string gFilenameMusic = "SuavementeMusic.wav";	// Name of the sound file (in project folder)
// std::string gFilenameDrum = "SuavementeDrum.wav";	// Name of the sound file (in project folder)

// Rain
// std::string gFilenameMusic = "RainMusic.wav";	// Name of the sound file (in project folder)
// std::string gFilenameDrum = "RainDrum.wav";	// Name of the sound file (in project folder)

// HeavyRotation
// std::string gFilenameMusic = "HeavyRotationMusic.wav";	// Name of the sound file (in project folder)
// std::string gFilenameDrum = "HeavyRotationDrum.wav";	// Name of the sound file (in project folder)

std::vector<float> gSampleBufferMusic;				// Buffer that holds the sound file
std::vector<float> gSampleBufferDrum;				// Buffer that holds the sound file

int gReadPointerMusic = 0;							// Position of the last frame we played 
float gAmplitudeMusic = 0.1;		// Amplitude of the sine wave (1.0 is maximum)

int gReadPointerDrum = 0;							// Position of the last frame we played 
float gAmplitudeDrum = 0.6;		// Amplitude of the sine wave (1.0 is maximum)

int gAudioFramesPerAnalogFrame = 0;		// the ratio of the audio sample rate to analog sample rate

int gChannelMusic = 0;
int gChannelDrum = 2;
bool flag = true;

bool setup(BelaContext *context, void *userData)
{
	if(context->analogSampleRate == context->audioSampleRate)
	{
		// gAudioFramesPerAnalogFrame = context->audioSampleRate / context->analogSampleRate;
	}else
	{
		std::cout
		<<"The ratio of the sampl rates are unexpected"
        <<"analog sample rate:"
        <<context->analogSampleRate
        <<"\n audio sample rat"
        <<context->audioSampleRate
        <<std::endl;
        
        return false;
	} // Just in case the sample rate is weird
	
	std::cout
        <<"analog sample rate:"
        <<context->analogSampleRate
        <<"\n audio sample rat"
        <<context->audioSampleRate
        <<std::endl;
        
    rt_printf("analog '%.1f'\n", context->analogSampleRate);
    rt_printf("audio '%.1f'\n", context->audioSampleRate);

	
	// Load the sample from storage into a buffer	
	gSampleBufferMusic = AudioFileUtilities::loadMono(gFilenameMusic);
	gSampleBufferDrum = AudioFileUtilities::loadMono(gFilenameDrum);
	
	// Check if the load succeeded
	if(gSampleBufferMusic.size() == 0) {
    	rt_printf("Error loading music file '%s'\n", gFilenameMusic.c_str());
    	return false;
	}
	
	// Check if the load succeeded
	if(gSampleBufferDrum.size() == 0) {
    	rt_printf("Error loading drim file '%s'\n", gFilenameDrum.c_str());
    	return false;
	}

    rt_printf("Loaded the music file '%s' with %d frames (%.1f seconds)\n", 
    			gFilenameMusic.c_str(), gSampleBufferMusic.size(), // the number of the samples
    			gSampleBufferMusic.size() / context->audioSampleRate); // the duration of the sound based on the audioSampleRate
    
    rt_printf("Loaded the drum file '%s' with %d frames (%.1f seconds)\n", 
    			gFilenameDrum.c_str(), gSampleBufferDrum.size(), // the number of the samples
    			gSampleBufferDrum.size() / context->audioSampleRate); // the duration of the sound based on the audioSampleRate

	return true;
}

void render(BelaContext *context, void *userData)
{
	if(flag)
	{
		rt_printf("analog '%.1f'\n", context->analogSampleRate);
    	rt_printf("audio '%.1f'\n", context->audioSampleRate);
    	flag = false;
	}
	
    for(unsigned int n = 0; n < context->audioFrames; n++) {
        // TODO: get the next sample from the buffer, then update the read pointer
        // When you reach the end of the buffer, start over again from the beginning.
		// Hint: use gSampleBuffer.size() to find the length of the buffer in samples.
        float outMusic = gAmplitudeMusic* gSampleBufferMusic[gReadPointerMusic];
        
        // if(gReadPointerMusic)
        float outDrum = gAmplitudeDrum* gSampleBufferDrum[gReadPointerMusic];
        
        gReadPointerMusic++;
        gReadPointerDrum++;
        
        if((gReadPointerMusic >= gSampleBufferMusic.size()) or (gReadPointerDrum >= gSampleBufferDrum.size()))
        	gReadPointerMusic = 0;
        	gReadPointerDrum = 0;
        	
        	

    	for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			// Write the sample to every audio output channel
    		audioWrite(context, n, channel, outMusic);
    	}
    	
    	// analogWriteOnce(context, n, 0, outDrum);
    	
    	for(unsigned int analogChannel = 0; analogChannel < gChannelDrum; analogChannel++) {
    		analogWriteOnce(context, n, analogChannel, outDrum);
    	}
    	
    }
    	
    for (unsigned int n = 0; n < context->analogFrames; n++){

		for (unsigned int channel = 0; channel < 2; channel++){
	
	
		}
    }
    
}

void cleanup(BelaContext *context, void *userData)
{

}
