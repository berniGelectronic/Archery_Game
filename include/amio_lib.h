//=============================================================================
//  Introduction to C Programming (Audio-MIDI Programming)
//=============================================================================
//  amio_lib API (Audio MIDI Input Output Library API)
//  File:   amio_lib.h
//  Author: Dimitri Zantalis
//  Date:   March 2013
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

#ifndef AMIO_LIB_H
#define AMIO_LIB_H

//=============================================================================
//	Include Files
//============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sndfile.h"
#include "portaudio.h"
#include "portmidi.h"
#include "porttime.h"

//=============================================================================
//  Define directives
//=============================================================================

//////////////////////////////
//AMIO_LIB DEFINE DIRECTIVES//
//////////////////////////////
#define MAX(a,b)           ( (a>b)?a:b )
#ifndef M_PI
#define M_PI  (3.14159265)
#endif

///////////////////////////////
//PORTAUDIO DEFINE DIRECTIVES//
///////////////////////////////
#define SYSTEM_SAMPLE_RATE   (44100.0)
#define FRAMES_PER_BUFFER   (512)

//////////////////////////////
//PORTMIDI DEFINE DIRECTIVES//
//////////////////////////////
#define MD_NOTEON 0x90
#define MD_NOTEOFF 0x80
#define MD_PRG  0xC0
#define MIDI_PITCHBEND  0xE0
#define SBYTE(mess,chan) mess | chan

//=============================================================================
//	Variables, Data Structure specific to amio_lib
//=============================================================================
/////////////////////////////////////////////////
//VARIABLES & DATA STRUCTS USED WITH LIBSNDFILE//
/////////////////////////////////////////////////

typedef int SfError;

//-----------------------------------------------------------------------------
//  SIGNAL structure to hold information about an audio file
//-----------------------------------------------------------------------------
/*!
 @struct SIGNAL
 @abstract      A basic structure used to hold information about an audio file.
 @discussion    This structure is fundamental to audio_lib. Each SIGNAL structure
represents an audio file; both its data and meta-parameters such as sampling rate,
number of channels etc.
 @field INFO    This is a sndlib specific structure and contains all important
 information about an audio file. In particular it contains:
    -number of frames in the file
    -sampling rate
    -number of channels
    -format (e.g PCM 32-bit float)
and some more. For more information please look into sndfile.h header file
and also read [2].
 @field FRAMES  The number of frames contained in the file (same as info.frames).
Usually a file containing sampled sound is split into frames, where each frame
comprises a number of samples, depending on the number of channels, the format of
the file and possibly other parameters. For example, in 1-channel(mono)
PCM (Pulse Code Modulation) 32-bit file, a frame will contain just one sample.
If the file has two channels then each frame will contain two samples. This is
true for PCM files but not for all formats. In formats with variable bit rates
(e.g. MP3) each frame might contain an arbitrary number of samples depending on
the encoding used. In such cases we determine the number of samples in a frame
by using other meta-parameters that are included in the file.
In this course we are only conserned with PCM formats.
 @field NAME    This is a pointer to a string containing the name of the file.
The name must be supplied by the programmer.
 @field DATA    a pointer to a buffer (some memory space) containing the samples
(data) of the audio file.
 @field MODE    amio_lib API specific field
 @field BLOCKSIZE   amio_lib API specific field

 */
typedef struct SIGNAL
{
    const char* name;
    sf_count_t frames;
    int samplerate;
    int channels;
    int format;
    int sections;
    int seekable;
    float *data;
} SIGNAL;

////////////////////////////////////////////////
//VARIABLES & DATA STRUCTS USED WITH PORTAUDIO//
////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//  AUDIOSTREAM structure to hold informatino about an audio stream
//-----------------------------------------------------------------------------
/*!
@struct AUDIOSTREAM
@abstract
@discussion
@field
@field
@field
*/
typedef struct AUDIOSTREAM
{
    PaStream *stream;
    float sampleRate;
    int outChannels;
    int inChannels;
    PaSampleFormat sampleFormat;
    unsigned long framesPerBuffer;
} AUDIOSTREAM;

///////////////////////////////////////////////
//VARIABLES & DATA STRUCTS USED WITH PORTMIDI//
///////////////////////////////////////////////
PmError retval;
PortMidiStream *mstream;

///////////////////////////////////////////////
//VARIABLES & DATA STRUCTS USED WITH AMIO_lIB//
///////////////////////////////////////////////
enum
{
    kAudioSystem=0,
    kMIDISystem,
    kSndFileSystem
} systemScope;


//=============================================================================
//	Function Prototypes
//=============================================================================

//////////////////////////////
//FUNCTIONS USING LIBSNDFILE//
//////////////////////////////
SfError wavread(const char* inputFileName, SIGNAL *input);
SfError wavwrite(const char* outputFileName, SIGNAL *output);
void Fill_SIGNAL(SIGNAL *fd, int frames, int fs, int channels, int format,
                 int sections, int seekable, float * data);
void displaySndInfo(SIGNAL *sig);
void compare(SIGNAL *original, SIGNAL *processed);

/////////////////////////////
//FUNCTIONS USING PORTAUDIO//
/////////////////////////////
PaError initialiseAudioSystem(void);
PaError terminateAudioSystem(void);
PaError openDefaultAudioStream(AUDIOSTREAM *stream,
                               PaStreamCallback *streamCallback,
                               void *userData);
PaError setAudioStreamFinishedCallback(AUDIOSTREAM *astream,
                                       PaStreamFinishedCallback *streamFinishedCallback);
PaError startAudioStream(AUDIOSTREAM *stream);
PaError stopAudioStream(AUDIOSTREAM *stream);
PaError closeAudioStream(AUDIOSTREAM *stream);

////////////////////////////
//FUNCTIONS USING PORTMIDI//
////////////////////////////
//The following functions were written by Dr. Julian Miller:
//pause function is commented out because there is a conflict with unistd.h when
//compiling for OSX and probably Linux. Use pause function in graphics_lib.h
//instead.
void pause(int time_in_milliseconds); /* a conflict */
void midi_note(int pitch, int channel, int velocity);
void program_change(int channel, int instrument);
void midi_start(void);
void midi_close(void);

//Extra functions written by Dimitri Zantalis
void initialiseMIDISystem(void);
void terminateMIDISystem(void);
void select_midi_in(PortMidiStream *instream);
void select_midi_out(PortMidiStream *outstream);
void close_midi_in(PortMidiStream *instream);
void close_midi_out(PortMidiStream *outstream);
void midi_read_message(PortMidiStream *instream);

/////////////////////////////
//GENERIC UTILITY FUNCTIONS//
/////////////////////////////
int random_number(int lower_range, int upper_range);//Written by Dr. Julian Miller
int checkErr(int err,int library,const char* message);



#endif //End of AUDIO_LIB_H
