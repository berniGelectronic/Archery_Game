//=============================================================================
//  Introduction to C Programming (Audio-MIDI Programming)
//=============================================================================
//  amio_lib API Function Implementations
//  File:   amio_lib.h
//  Author: Dimitri Zantalis
//  Date:   Oct 2013
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

#include <amio_lib.h>

//=============================================================================
//	Function Implementations
//=============================================================================

////////////////////////////////
//LIBSNDFILE RELATED FUNCTIONS//
////////////////////////////////

//-----------------------------------------------------------------------------
//  wavread Function to read audio file into memory
//-----------------------------------------------------------------------------
/*!
 @function      wavread
 @abstract      Read an audio file into memory.
 @discussion    An utility function that reads the contents of an audio file
 into a SIGNAL structure. It uses and depends on the libsndfile API.
 @param INPUTFILENAME  The filename to use for the input file. If no path is
 provided the file will be read from the location of the executable.
 @param INPUT   A SIGNAL structure containing the audio data read from the audio file.
 @result SfError a libsndfile error code (an integer).
 */
SfError wavread(const char* inputFileName, SIGNAL *input)
{
    int err=0;
    SNDFILE *file=NULL;
    SF_INFO info;
    sf_count_t framesread=0;
    info.format=0;

    //Open a file for reading.
    if(!(file=sf_open(inputFileName, SFM_READ, &info)))
    {
        printf("Failed to open '%s' for reading!\n",inputFileName);
        err=sf_error(file);
        checkErr(err,kSndFileSystem,"wavread: opening file");
        return err; //Return at this point because the file did not open, so there is nothing we can do!
    }

    //Check if SF_INFO struct is valid
    //err=sf_format_check(&info);
    //checkErr(err,kSndFileSystem,"wavread: format checking code");

    //Create a buffer to hold audio data.
    float *buffer=(float *)malloc(sizeof(float)* info.frames*info.channels);

    //Read audio data into buffer;
    framesread=sf_readf_float(file, buffer, info.frames);
    if(framesread==0)
    printf("No frames were read from the file! The file is either corrupt or the operation failed!\n");
    err=sf_error(file);
    checkErr(err,kSndFileSystem,"wavread: reading audio data into buffer");

    //Close audio file.
    err=sf_close(file);
    checkErr(err,kSndFileSystem,"wavread: closing file");

    //Set up SIGNAL structure based on aqquired SF_INFO
    input->name=inputFileName; //Name of the input signal procided by the user
    input->frames=info.frames; //Number of frames of the audio file
    input->samplerate=info.samplerate; //Sample rate of audio file
    input->channels=info.channels; //Number of channels of audio file
    input->format=info.format; //Format of audio file
    input->sections=info.sections; //Sections of audio file
    input->seekable=info.seekable; //One ('1') if audio file is seekable, zero otherwise
    input->data=buffer; //The buffer that contains the actual audio data/samples of the audio file

    return err;
}

//-----------------------------------------------------------------------------
//  wavwrite function to write audio file into disk
//-----------------------------------------------------------------------------
/*!
 @function      wavwrite
 @abstract      Write a SIGNAL (audio file) to disk
 @discussion    Utility function that writes the contents of a SIGNAL structure
 to disk. It uses and depends on the libsndfile API.
 @param OUTPUTFILENAME  The filename to use for the output file. If no path is
 provided the file will be saved at the location the executable resides.
 @param OUPUT   A SIGNAL structure containing the audio data to write to disk.
 @result SfError a libsndfile error code (an integer).
 */
SfError wavwrite(const char* outputFileName, SIGNAL *output)
{

    int err=0;
    SNDFILE* file=NULL;
    SF_INFO info;
    sf_count_t frameswritten=0;

    info.samplerate=output->samplerate;
    info.channels=output->channels;
    info.format=output->format;

    //Check if SF_INFO struct is valid
    //err=sf_format_check(&info);
    //checkErr(err,kSndFileSystem,"wavwrite: format checking code");

    //Open a file for writting
    if(!(file=sf_open(outputFileName, SFM_WRITE, &info)))
    {
        printf("Failed to open %s for writting!\n",outputFileName);
        err=sf_error(file);
        checkErr(err,kSndFileSystem,"wavwrite: opening file");
        return err;//Return at this point because the file could not open so we cannot write anything!
    }

    //Write audio data to audio file.
    frameswritten=sf_writef_float(file, output->data, output->frames);
    if(frameswritten!=output->frames)
    {
        err=sf_error(file);
        checkErr(err,kSndFileSystem,"wavwrite: writting data to file");
    }

    //Close audio file.
    err=sf_close(file);
    checkErr(err,kSndFileSystem,"wavwrite: closing file");

    return err;
}

//-----------------------------------------------------------------------------
//  Fill_SIGNAL Utility function to fill in the fields of a SIGNAL struct
//-----------------------------------------------------------------------------
/*!
 @function      Fill_SIGNAL
 @abstract      This function fills up the field of a SIGNAL struct.
 @discussion	Call this to fill up a SIGNAL struct. This might be needed when
 trying to write to an output file. Before doing so we need to provide a ready made
 SIGNAL struct that contains vital information about the file to be written such as
 format, sample rate, number of channels and the data to write to the file.
 @param
 @result VOID
 */
void Fill_SIGNAL(SIGNAL *fd,
                 int frames,
                 int fs,
                 int channels,
                 int format,
                 int sections,
                 int seekable,
                 float * data)
{
    fd->frames=frames;
    fd->samplerate=fs;
    fd->channels=channels;
    fd->format=format;
    fd->sections=sections;
    fd->seekable=seekable;
    fd->data=data;
}

//-----------------------------------------------------------------------------
//  displaySndInfo Utility function to display the fields of an SF_INFO struct
//-----------------------------------------------------------------------------
/*!
 @function      displaySndInfo
 @abstract      Display information about the audio file
 @discussion    This is a helper function which displays
 @param SIGNAL  A SIGNAL structure. Make sure the SIGNAL structure is initialised
 before passing as argument to this function, otherwise the results are undetrmined.
 @result VOID
 */
void displaySndInfo(SIGNAL *sig)
{

    printf("----------------------\n");
    printf("%s Information\n",sig->name);
    printf("Number of frames: %I64d\n",sig->frames);
    printf("Sample Rate: %d\n",sig->samplerate);
    printf("Number of channels: %d\n",sig->channels);
    printf("Format: %08x\n",sig->format);
    printf("Sections: %d\n",sig->sections);
    printf("Seekable?: %d\n",sig->seekable);
    printf("----------------------\n");
}

//-----------------------------------------------------------------------------
//  Compare Utility function to compare two signals
//-----------------------------------------------------------------------------
/*!
 @function      copare
 @abstract      Compare similarity of two signals.
 @discussion
 @param ORIGINAL A buffer containing samples of the original signal.
 @param COMPUTED A buffer containing samples of the processed signal.
 @param LENGTH The size of the buffers.
 @result VOID
 */
void compare(SIGNAL *original, SIGNAL *processed)
{
    int             i;
    float *_original=original->data;
    float *_processed=processed->data;
    float           error = _original[0] - _processed[0];
    float           max = error;
    float           min = error;
    float           mean = 0.0;
    float           sd_radicand = 0.0;
    sf_count_t length=original->frames;

    for (i = 0; i < length; i++)
    {
        error = _original[i] - _processed[i];
        /* printf("%f %f %f\n", _original[i], _processed[i], error); */
        max = (max < error) ? error : max;
        min = (min > error) ? error : min;
        mean += (error / length);
        sd_radicand += ((error * error) / (float) length);
    }

    printf("Max error: %f  Min error: %f  Mean: %f  Std Dev: %f\n",
           max, min, mean, sqrt(sd_radicand));
}

//////////////////////////////
//PORTAUDIO RELATED FUNCTIONS//
//////////////////////////////

//-----------------------------------------------------------------------------
//  initialiseAudioSystem function
//-----------------------------------------------------------------------------
/*!
 @function initialiseAudioSystem
 @abstract
 @discussion
 @param
 @result
 */
PaError initialiseAudioSystem(void)
{
    PaError err=0;

    //Initialise PortAudio engine.
    err = Pa_Initialize();

    return err;
}

//-----------------------------------------------------------------------------
//  terminateAudioSystem function
//-----------------------------------------------------------------------------
/*!
 @function terminateAudioSystem
 @abstract
 @discussion
 @param
 @result
 */
PaError terminateAudioSystem(void)
{
    PaError err=0;

    //Terminate PortAudioEngine.
    err=Pa_Terminate();

    return err;
}

//-----------------------------------------------------------------------------
//  openDefaultAudioStream function
//-----------------------------------------------------------------------------
/*!
 @function openDefaultAudioStream
 @abstract
 @discussion
 @param
 @result
 */
PaError openDefaultAudioStream(AUDIOSTREAM *audioStream,
                               PaStreamCallback *streamCallback,
                               void *userData)
{
    PaError err=0;
    PaStreamParameters outputParameters;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice)
    {
        fprintf(stderr,"Error: No default output device.\n");
        printf("Application will exit.");
        exit(EXIT_FAILURE);
    }
    outputParameters.channelCount = audioStream->outChannels;
    outputParameters.sampleFormat = audioStream->sampleFormat;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &audioStream->stream,
              NULL, /* no input */
              &outputParameters,
              audioStream->sampleRate,
              audioStream->framesPerBuffer,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              streamCallback,
              userData);
    return err;
}

//-----------------------------------------------------------------------------
//  setAudioStreamFinishedCallback function
//-----------------------------------------------------------------------------
/*!
 @function setAudioStreamFinishedCallback
 @abstract
 @discussion
 @param
 @result
 */
PaError setAudioStreamFinishedCallback(AUDIOSTREAM *astream,
                                       PaStreamFinishedCallback *streamFinishedCallback)
{
    PaError err=0;

    err = Pa_SetStreamFinishedCallback( astream->stream, streamFinishedCallback );

    return err;
}
//-----------------------------------------------------------------------------
//  openDefaultAudioStream function
//-----------------------------------------------------------------------------
/*!
 @function openDefaultAudioStream
 @abstract
 @discussion
 @param
 @result
 */
PaError startAudioStream(AUDIOSTREAM *stream)
{
    PaError err=0;

    //Start the audio stream
    err=Pa_StartStream(stream->stream);

    return err;
}
//-----------------------------------------------------------------------------
//  stopAudioStream function
//-----------------------------------------------------------------------------
/*!
 @function stopAudioStream
 @abstract
 @discussion
 @param
 @result
 */
PaError stopAudioStream(AUDIOSTREAM *stream)
{
    PaError err=0;

    //Stop the audio stream
    err=Pa_StopStream(stream->stream);

    return err;
}

//-----------------------------------------------------------------------------
//  closeAudioStream function
//-----------------------------------------------------------------------------
/*!
 @function closeAudioStream
 @abstract
 @discussion
 @param
 @result
 */
PaError closeAudioStream(AUDIOSTREAM *stream)
{
    PaError err=0;

    //Close the audio stream
    err=Pa_CloseStream(stream->stream);

    return err;
}

//////////////////////////////
//PORTMIDI RELATED FUNCTIONS//
//////////////////////////////

//-----------------------------------------------------------------------------
//  pause utility function
//-----------------------------------------------------------------------------
/*!
 @function pause
 @abstract
 @discussion
 @param
 @result
 */

void pause(int time_in_milliseconds)
{
    long time;

    time = Pt_Time(NULL);
    while(Pt_Time(NULL) - time < time_in_milliseconds);
}

//-----------------------------------------------------------------------------
//  midi_note function
//-----------------------------------------------------------------------------
/*!
 @function midi_note
 @abstract
 @discussion
 @param
 @result
 */
void midi_note(int pitch, int channel, int velocity)
{
    int channel1,  velocity1;

    channel1 = 1 + (channel % 16);
    velocity1 = velocity % 128;

    Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_NOTEON,channel1), pitch, velocity1));
}

//-----------------------------------------------------------------------------
//  program_change function
//-----------------------------------------------------------------------------
/*!
 @function program_change
 @abstract
 @discussion
 @param
 @result
 */
void program_change(int channel, int instrument)
{
    int channel1, instrument1;

    instrument1 = 1+ (instrument % 128);
    channel1 = 1 + (channel % 16);

    Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_PRG,channel1), instrument1, 0));
}

//-----------------------------------------------------------------------------
//  midi_start function
//-----------------------------------------------------------------------------
/*!
 @function midi_start
 @abstract
 @discussion
 @param
 @result
 */
void midi_start()
{
    Pm_Initialize();
    Pt_Start(1, NULL, NULL);

    retval = Pm_OpenOutput(&mstream, 0,NULL,512,NULL,NULL,0);
    if(retval != pmNoError)
    {
        printf("error: %s \n", Pm_GetErrorText(retval));
    }
    else /* set channel 1 to grand piano */
        program_change(1, 1);
}

//-----------------------------------------------------------------------------
//  midi_close function
//-----------------------------------------------------------------------------
/*!
 @function midi_close
 @abstract
 @discussion
 @param
 @result
 */
void midi_close(void)
{
    Pm_Close(mstream);
    Pm_Terminate();
}

//-----------------------------------------------------------------------------
//  initialiseMIDISystem function
//-----------------------------------------------------------------------------
/*!
 @function initialiseMIDISystem
 @abstract
 @discussion
 @param
 @result
 */
void initialiseMIDISystem(void)
{
    Pm_Initialize();
}

//-----------------------------------------------------------------------------
//  terminateMIDISystem function
//-----------------------------------------------------------------------------
/*!
 @function terminateMIDISystem
 @abstract
 @discussion
 @param
 @result
 */
void terminateMIDISystem(void)
{
    Pm_Terminate();
}

//-----------------------------------------------------------------------------
//  select_midi_in function
//-----------------------------------------------------------------------------
/*!
 @function select_midi_in
 @abstract
 @discussion
 @param
 @result
 */
void select_midi_in(PortMidiStream *instream)
{
    int i;
    int nod;
    int midiin;
    const PmDeviceInfo * di;

    //Get number of devices
    nod=Pm_CountDevices();

    //Display only MIDI IN devices
    printf("MIDI IN Devices\n----------------\n");
    for(i=0; i<nod; i++)
    {
        di=Pm_GetDeviceInfo(i);
        if(di->input>0)
        {
            printf("Device ID: %d\n",i);
            printf("Intreface: %s\n     Name: %s\n",di->interf,di->name);
            printf("   Inputs: %d\n",di->input);
            //printf("   Inputs: %d\n  Outputs: %d\n",(di->input>0)?di->input:0,(di->output>0)?di->output:0);
            printf("  Opened?: %s\n\n",(di->opened)?"true":"false");
        }
    }

    do
    {
        printf("Select MIDI IN Devide ID: ");
        scanf("%d",&midiin);
        printf("%d\n\n",midiin);
        retval = Pm_OpenInput(&instream, midiin,NULL,512,NULL,NULL);
        if(retval != pmNoError)
        {
            printf("error: %s \n\n", Pm_GetErrorText(retval));
        }
        else
        {
            printf("Selected device %d\n\n",midiin);
        }
    }
    while(retval != pmNoError); //while((midiin<0|midiin>nod-1)|(di->input==0));
}

//-----------------------------------------------------------------------------
//  select_midi_out function
//-----------------------------------------------------------------------------
/*!
 @function select_midi_out
 @abstract
 @discussion
 @param
 @result
 */
void select_midi_out(PortMidiStream *outstream)
{
    int i;
    int nod;
    int midiout;
    const PmDeviceInfo *di;

    //Get number of devices
    nod=Pm_CountDevices();

    //Display only MIDI OUT devices
    printf("MIDI OUT Devices\n----------------\n");
    for(i=0; i<nod; i++)
    {
        di=Pm_GetDeviceInfo(i);
        if(di->output>0)
        {
            printf("Device ID: %d\n",i);
            printf("Intreface: %s\n     Name: %s\n",di->interf,di->name);
            printf("  Outputs: %d\n",di->output);
            //printf("   Inputs: %d\n  Outputs: %d\n",(di->input>0)?di->input:0,(di->output>0)?di->output:0);
            printf("  Opened?: %s\n\n",(di->opened)?"true":"false");
        }
    }

    do
    {
        printf("Select MIDI OUT Devide ID: ");
        scanf("%d",&midiout);

        retval = Pm_OpenOutput(&outstream, midiout,NULL,512,NULL,NULL,0);
        if(retval != pmNoError)
        {
            printf("error: %s \n\n", Pm_GetErrorText(retval));
        }
        else
        {
            printf("Selected device %d\n\n",midiout);
        }
    }
    while(retval != pmNoError); //while((midiin<0|midiin>nod-1)|(di->input==0));

}

//-----------------------------------------------------------------------------
//  close_midi_in function
//-----------------------------------------------------------------------------
/*!
 @function close_midi_in
 @abstract
 @discussion
 @param
 @result
 */
void close_midi_in(PortMidiStream *in)
{
    Pm_Close(in);
}

//-----------------------------------------------------------------------------
//  close_midi_out function
//-----------------------------------------------------------------------------
/*!
 @function close_midi_out
 @abstract
 @discussion
 @param
 @result
 */
void close_midi_out(PortMidiStream *out)
{
    Pm_Close(out);
}

/////////////////////////////
//GENERIC UTILITY FUNCTIONS//
/////////////////////////////

//-----------------------------------------------------------------------------
//  random_number utility function that generates a random number within a
//  given range.
//-----------------------------------------------------------------------------
/*!
 @function      random_number
 @abstract      Generate a random number within a given range.
 @discussion
 @param
 @result
 */
int random_number(int lower_range, int upper_range)
{
    int x;

    if (upper_range < lower_range)
    {
        x = upper_range;
        upper_range = lower_range;
        lower_range = x;
    }
    return lower_range + rand() %(upper_range -lower_range +1);

}


int checkErr(int err,int systemScope,const char* message)
{

    switch(systemScope)
    {
    case kAudioSystem:
        if(err != paNoError)
            printf("Audio system: %s [%s]\n", Pa_GetErrorText(err),message);
        break;

    case kMIDISystem:
        if(err != pmNoError)
            printf("MIDI system: %s [%s]\n", Pm_GetErrorText(err),message);
        break;

    case kSndFileSystem:
        if(err != 0)
            printf("Sound File system: %s [%s]\n", sf_error_number(err),message);
        break;

    default:
        printf("Error: %d\n",err);
        break;
    }

    return err;
}
