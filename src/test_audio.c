//=============================================================================
//  Introduction to C Programming (Audio Programming Exercises)
//=============================================================================
//  audio_lib API
//  File:   ex_fileplayback.c
//  Author: Dimitri Zantalis
//  Date:   September 2013
//
//=============================================================================
//  Discussion
//=============================================================================
//
//
//=============================================================================
//  Copyright and License Information
//=============================================================================
//  Copyright (c) 2013 Dimitri Zantalis <dz548@york.ac.uk>
//
//  For license information see 'include\license.h'
//
//=============================================================================
//	Recommended Reading List & References
//=============================================================================
//  [1] libsndfile, C library for reading/writing audio files,
//      http://www.mega-nerd.com
//
//  [2] PortAudio, 'Portable Cross-platform Audio I/O, http://www.portaudio.com
//
//  [3] PortMidi, 'Portable MIDI I/O library', http://portmedia.sourceforge.net/portmidi

#include "amio_lib.h"
#include "conio.h"
#include "archery.h"


#define VOLUME_INCREMENT (0.1f)
#define PAN_INCREMENT (0.1f)

typedef struct
{
    SIGNAL drums;
    long int ridx;
    float volume;
    float pan;
    char message[20];
}
USERDATA;

//-----------------------------------------------------------------------------
// This routine will be called by the PortAudio engine when audio is needed.
// It may called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().
//-----------------------------------------------------------------------------
static int processingCallback( const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData )
{
    USERDATA *data = (USERDATA*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;
    float aL,aR;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;


    for( i=0; i<framesPerBuffer; i++ )
    {
        //Apply panning and volume
        aL=(1.0f-data->pan)*data->volume;
        aR=(1.0f-aL)*data->volume;

        *out++ = aL*data->drums.data[data->ridx];  /* left */
        *out++ = aR*data->drums.data[data->ridx];  /* right */

        data->ridx++;
        if(data->ridx>=data->drums.frames)
            data->ridx=0;
    }

    return paContinue;
}

//-----------------------------------------------------------------------------
//This routine is called by portaudio when playback is done.
//-----------------------------------------------------------------------------
static void audioStreamFinished( void* userData )
{
    USERDATA *data = (USERDATA *) userData;
    printf( "Stream Completed: %s\n", data->message );
}

void main_audio(sounds_t sound_SFX)
{
    //=============================================================================
    //Variables and Data Structures
    //=============================================================================
    PaError err=0;
    char key;
    char ctrl;
    USERDATA data;
    AUDIOSTREAM outStream;
    int delay;
    char label[20];

    //=============================================================================
    //STEP 0 - Preliminary set-up and initialisation
    //=============================================================================

    //Set up AUDIOSTREM structure.
    outStream.stream=NULL;
    outStream.sampleRate=44100;
    outStream.sampleFormat=paFloat32;
    outStream.inChannels=0;
    outStream.outChannels=2;
    outStream.framesPerBuffer=1024;
    data.ridx=0;
    data.volume=0.5;
    data.pan=0.5;
//INTRO,
//    CROWD,
//    ARROW_HIT,
//    ARROW_FLY,
//    BOW_PREPARATION.
//    MISS_TARGET,
//    GOOD,
//    STAY_FOCUSED
    switch(sound_SFX)
    {
    case INTRO:
        sprintf(label,"%s","samples/intro.wav");
        delay=1200;
        outStream.sampleRate=88200;
        break;
    case CROWD:
        sprintf(label,"%s","samples/hit_crowd.wav");
        delay=2000;
        outStream.sampleRate=16000;
        break;
    case ARROW_HIT:
        sprintf(label,"%s","samples/arrow_hit.wav");
        delay=601;
        outStream.sampleRate=44100;
        break;
    case ARROW_FLY:
        sprintf(label,"%s","samples/arrow_fly.wav");
        delay=253;
        outStream.sampleRate=44100;
        break;
    case BOW_PREPARATION:
        sprintf(label,"%s","samples/bow_prep.wav");
        delay=511;
        outStream.sampleRate=44100;
        break;
    case MISS_TARGET:
        sprintf(label,"%s","samples/miss_target.wav");
        delay=1600;
        outStream.sampleRate=44100;
        break;
    case STAY_FOCUSED:
        sprintf(label,"%s","samples/stay_focused.wav");
        delay=1200;
        outStream.sampleRate=44100;
        break;
    case GOOD:
        sprintf(label,"%s","samples/good.wav");
        delay=780;
        outStream.sampleRate=44100;
        break;
    case CLICK:
        sprintf(label,"%s","samples/click.wav");
        delay=100;
        outStream.sampleRate=88200;
        break;


    }

    //Read audio file into memory (i.e into 'data.drums' SIGNAL structure).
    err=wavread(label,&data.drums);
    checkErr(err,kSndFileSystem,"Failed to read file into memory.");

    //=============================================================================
    //STEP 1 - Initialise audio system
    //=============================================================================
    err=initialiseAudioSystem();
    err=checkErr(err,kAudioSystem,"Failed to initialise audio system.");
    if(err!=0)
    {
        printf("Application will exit.");
        exit(EXIT_FAILURE);
    }

    //=============================================================================
    //STEP 2 - Open an audio stream
    //=============================================================================
    err=openDefaultAudioStream(&outStream,processingCallback,&data);
    checkErr(err,kAudioSystem,"Failed to open default audio stream.");

    //Register 'audioStreamFinished' callback with our stream.
    //This callback will be called as sson as the audio stream is stopped.
    err=setAudioStreamFinishedCallback(&outStream,&audioStreamFinished);

    //=============================================================================
    //STEP 3 - Main control loop
    //=============================================================================


    printf("Audio stream playing...\n\n");
    err=startAudioStream(&outStream);
    checkErr(err,kAudioSystem,"Failed to start audio stream.");
    data.volume=1.0f;
    data.pan=0.5f;
    pausefor(delay);

    err=closeAudioStream(&outStream);
    checkErr(err,kAudioSystem,"Failed to close audio stream.");

    //=============================================================================
    //PART 4 - Clean up
    //=============================================================================
    err=terminateAudioSystem();
    checkErr(err,kAudioSystem,"Failed to terminate audio system.");
//    return err;
}
int main()
{
    int i;
//    sounds_t sfx[8];

    for(i=0; i<8; i++)
    {
        main_audio(CLICK);
//        break;
        pausefor(500);
    }

//    main_audio(INTRO);
    return 0;
}
