/* Sample assignment: really it is a combination
   of graphics1, music1 and audio1  */

#include <graphics_lib.h>
#include <amio_lib.h>

#define kInputSignalLocation "samples/drums.wav"
#define kOutputSignalLocation "samples/drums_out_01.wav"

int audio_main(void)
{
    //=============================================================================
    //STEP 1 - INITIALISATION
    //=============================================================================
    int err=0;

    //Create two SIGNAL structures. These structures contain important
    //information about the signals that will be used (input and output).
    SIGNAL input,output;

    //Initialise SIGNAL structures.
    //Set the memory space occupied by the structures to zero.
    //Although this step is not always necessary,
    //it is good programming practice to initialise all variables, structures etc.
    memset(&input,0,sizeof(SIGNAL));
    memset(&output,0,sizeof(SIGNAL));

    //=============================================================================
    //STEP 2 - OPEN AUDIO FILE AND READ INTO MEMORY
    //=============================================================================
    //Read input signal into memory.
    //The input signal is loaded into the memory location
    //pointed by the 'input.data' structure field (which is a pointer to an array of floats).
    printf("Reading '%s' into memory...\n",kInputSignalLocation);
    err=wavread(kInputSignalLocation, &input);
    checkErr(err,kSndFileSystem,"Failed to read audio file into memory");

    //Display information about the input signal.
    printf("\n\n");
    displaySndInfo(&input);

    //=============================================================================
    //STEP 3 - PREPARE THE OUTPUT SIGNAL
    //=============================================================================
    //Set up output SIGNAL structure. We want to write the output signal
    //to a file so we need to set up the SIGNAL structure with
    //the file format, sampling rate and some other
    //parameters of the output file that we want. In this case we only want to
    //copy the input to the output so we set up the output SIGNAL structure
    //with the same state as the input SIGNAL structure.
    Fill_SIGNAL(&output,
                input.frames,
                input.samplerate,
                input.channels,
                SF_FORMAT_WAV|SF_FORMAT_PCM_32,
                input.sections,
                input.seekable,
                NULL);

    //=============================================================================
    //STEP 4 - PROCESSING
    //=============================================================================
    //This is where signal processing takes place. In our case
    //we want to copy the input into the output, unmodified.
    //The only thing we have to do is to assign the data field of the input SIGNAL
    //to the data field of the output SIGNAL
    output.data=input.data; //Please note this is a pointer assignment

    //=============================================================================
    //STEP 5 - WRITE OUTPUT TO AUDIO FILE
    //=============================================================================
    //Write output file to disk
    printf("\n\nWriting audio to output file %s\n",kOutputSignalLocation);
    err=wavwrite(kOutputSignalLocation, &output);
    checkErr(err,kSndFileSystem,"Failed to write audio file to disk");

    //=============================================================================
    //STEP 6 - CLEAN UP
    //=============================================================================
    //We have now fnished with our files, but before we exit we need
    //to clean up any memory we used. We use the function 'free' for this task.
    free(input.data);
    //free(output.data); No need to free output.data because this point to
    //input.data which at this point is already deleted. Trying to delete
    //output.data will result in a crash.

    return 0;
}

/*
 * The midi main function - the program starts executing here
 */
int midi_main(void)
{
	/* Declare integer variables for specifying a note */
	int pitch, channel, velocity;

    /* initialize the midi functions */
    midi_start();

	/* Set the pitch variable to 60, which is middle C */
	pitch = 72;

	/* We will play the note on MIDI channel 1 */
	channel = 1;

	/* The note will have a medium velocity (volume) */
	velocity = 64;

	/* Start playing a middle C at moderate volume */
	midi_note(pitch, channel, velocity);

	/* Wait, for 1 second, so that we can hear the note playing */
	pause(1000);

	/* Turn the note off by setting its volume to 0 */
	midi_note(pitch, channel, 0);

    /* close down all midi functions */
    midi_close();

	return 0;
}

/* this is graphics1 main but with calls to midi and audio */
int main(void)
{
    /* Declare two variables for the x and y positions */
    int x_position, y_position;

	/* Open a graphics window */
	/* Make it 640 pixels wide by 480 pixels high */
	initwindow(640, 480);

	/* Set up some coordinates */
	x_position = 100;
	y_position = 340;

    /* choose red pen colour */
	setcolor(RED);

    /* draw a circle on the screen buffer
     at x_position, y_position
     with radius 10 and line thickness 2 */
    circle(x_position, y_position, 10, 2);

    /* move the contents of the screen buffer to the display */
    update_display();

	/* Wait for a key press */
	printf("Press a key to continue\n");
    getch();

    printf("\nNow playing a note using midi\n");
    midi_main();

    printf("\nNow running audio main\n");
    audio_main();

    /* remove the display */
    closegraph();


    return 0;
}
