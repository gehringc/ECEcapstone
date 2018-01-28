/*
FINAL MASTER PROGRAM FOR PORTABLE AUDIO WORKSTATION CAPSTONE PROJECT

*/

#include <Energia.h>
#include <pins_energia.h>
#include <Wire.h>
#include "AidaFW.h"
#include "AidaDSP.h"
#include <math.h>

#define EVER (;;)

// GLOBAL VARIABLES

uint8_t count = 0;
uint8_t restore = OFF;
uint8_t restoreflag = false;
uint16_t PotValue = 0;
uint32_t timec=0, prevtimec=0;

float chorusMix = 0.00;
float volume = 0.00;
float cVolume = 0.00;
float dVolume = 0.00;
float eVolume = 0.00;
float fVolume = 0.00;
float gVolume = 0.00;
float aVolume = 0.00;
float bVolume = 0.00;
float c5Volume = 0.00;

float sinVol = 0.00;
float sawVol = 0.00;
float sqrVol = 0.00;
float triVol = 0.00;

int frequency = 0.00;
float MaxVolume = 0.00;
float readback1 = 0.00;
float readback2 = 0.00;
float readback3 = 0.00;
float readback4 = 0.00;
float bitrange = 4095.00;
int forceResistC = 0;
int forceResistD = 0;
int forceResistE = 0;
int forceResistF = 0;
int forceResistG = 0;
int forceResistA = 0;
int forceResistB = 0;
int forceResistC5 = 0;
int chorus = 0;
int chrfb = 0;
int chrlfo = 0;
int vol = 0;
int octaveUp = 0;
int octaveDown = 0;
int prevOctU = 0;
int prevOctD = 0;
int flag = 0;
float frequencyAdj = 0.00;

int sawToggle = 0;
int sinToggle = 0;
int sqrToggle = 0;
int triToggle = 0;

//Initialize State toggles
int newStateSin =0;
int newStateSaw =0;
int newStateSqr =0;
int newStateTri =0;
int prevStateSin=0;
int prevStateSaw=0;
int prevStateSqr=0;
int prevStateTri=0;
int lineInSel = 0;
int preSel = 0;
int select = 0;
uint8_t channel = 1;

float freqC = 261.63;
float freqD = 293.66;
float freqE = 329.63;
float freqF = 349.23;
float freqG = 392.00;
float freqA = 440.00;
float freqB = 493.88;
float freqC5 = 523.25;

float cutoff = 0.00;
int cutFreq = 0;


void setup()
{
    // I/O
  pinMode(PM_1, INPUT); // Sin Toggle
  pinMode(PK_5, INPUT); // Saw Toggle
  pinMode(PK_4, INPUT); // Square Toggle
  pinMode(PM_0, INPUT); // Tri Toggle
  pinMode(PE_4, INPUT); // C
  pinMode(PE_5, INPUT); // D
  pinMode(PD_3, INPUT); // E
  pinMode(PE_0, INPUT); // F
  pinMode(PE_1, INPUT); // G
  pinMode(PE_2, INPUT); // A
  pinMode(PE_3, INPUT); // B
  pinMode(PD_7, INPUT); // C5
  pinMode(PB_5, INPUT); // Freq. Mod.
  pinMode(PD_2, INPUT); // Master Vol
  pinMode(PK_2, INPUT); // LP Cutoff
  pinMode(PK_3, INPUT); // Chorus Mix
  pinMode(PD_4, INPUT); // Chorus Feedback
  pinMode(PD_5, INPUT); // Chorus LFO
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(PA_7, INPUT);
  pinMode(PM_6, INPUT);
  pinMode(PB_4, INPUT); //Line-In-Mix
  pinMode(PK_7, INPUT); //line in effects?
  pinMode(PK_0, INPUT);
  // DSP board
  InitAida();	// Initialize DSP board
  digitalWrite(RESET, HIGH); // Wake up DSP
  delay(100);  // Start-up delay for DSP
  program_download();    // Here we load program, parameters and hardware configuration to DSP
}

void loop()
{
  //main code here, to run repeatedly:
  
  //Read potentiometer for control master volume
    
    vol=analogRead(PD_2);   //Read in pot pin to an int
    volume=vol;   //Int to Float
    volume=(bitrange-volume)/(bitrange);
    
  //Read pot for line in vol
    int lineVol = analogRead(PK_0);
    float lineV = lineVol;
    lineV = (bitrange-lineV)/bitrange;
    
   //Set volume at DSP
    MasterVolumeStereo(DEVICE_ADDR_7bit, MasterVolume, volume);  
   
   //Function that enables/disables waveforms, line in, serial data for loop 
    LineInMix(DEVICE_ADDR_7bit, NxMMixer1, sawVol, sinVol, sqrVol, triVol, lineV, 0.75); 
    
   //Enable/disable line in effects
    lineInSel = digitalRead(PK_7);
    if(lineInSel == 1 && preSel == 0){
        // button has been pressed
        if (channel == 1){
          channel = 2;
        }
        else if (channel == 2){
          channel = 1;
        }
     }
     preSel = lineInSel;
     
     //use a switch to either bypass or mix in effect chain
     mux(DEVICE_ADDR_7bit, LineSelect, channel, 2);
    
    
   //Enable disable waveform generation
    
    //Read values on pins for buttons
    newStateSin = digitalRead(PM_1);
    newStateSaw = digitalRead(PK_5);
    newStateSqr = digitalRead(PK_4);
    newStateTri = digitalRead(PM_0);
     
    //create software latch to toggle waveforms on/off  
    if(newStateSin == 1 && prevStateSin == 0){
        // button has been pressed
        sinToggle = !sinToggle;
        if (sinToggle == 1){
          sinVol = 1.00;
        }
        else {
          sinVol = 0.00;
        }
    }
    
    if(newStateSaw == 1 && prevStateSaw == 0){
        // button has been pressed
        sawToggle = !sawToggle;
        if (sawToggle == 1){
          sawVol = 1.00;
        }
        else {
          sawVol = 0.00;
        }
    }
    
    if(newStateSqr == 1 && prevStateSqr == 0){
        // button has been pressed
        sqrToggle = !sqrToggle;
        if (sqrToggle == 1){
          sqrVol = 1.00;
        }
        else {
          sqrVol = 0.00;
        }
    }
    
    if(newStateTri == 1 && prevStateTri == 0){
        // button has been pressed
        triToggle = !triToggle;
        if (triToggle == 1){
          triVol = 1.00;
        }
        else {
          triVol = 0.00;
        }
    }
    
    prevStateSin = newStateSin;
    prevStateSaw = newStateSaw;
    prevStateSqr = newStateSqr;
    prevStateTri = newStateTri;
      
        //KEYS AND OTHER PITCH MANIPULATION
        //Read in values for force sensitive resitors
        
        forceResistC = analogRead(PE_4); 
        forceResistD = analogRead(PE_5); //FINAL PIN
        forceResistE = analogRead(PD_3); //FINAL PIN
        forceResistF = analogRead(PE_0); //FINAL PIN
        forceResistG = analogRead(PE_1); //FINAL PIN
        forceResistA = analogRead(PE_2); //FINAL PIN
        forceResistB = analogRead(PE_3); //FINAL PIN
        forceResistC5 = analogRead(PD_7); 
        
        //Input pot value for cutoff frequency pf LP filter
        frequency = analogRead(PB_5);
        cutFreq = analogRead(PK_2);
        cutoff = cutFreq;
        cutoff = (4095.00 - (cutoff-100))/bitrange;
        //LP filter
        LPfilter(DEVICE_ADDR_7bit, Gen1stOrder1, cutoff);
        
        
        //CHORUS
        chorus = analogRead(PK_3);
        chrfb = analogRead(PD_4);
        chrlfo = analogRead(PD_5);
        chorusMix = chorus;
        chorusMix = (bitrange - chorusMix)/bitrange; 
        float fb = chrfb/(bitrange);
        float lfo = (1-chrlfo)/40.95;
        VocalChorus(DEVICE_ADDR_7bit, VChor1, NxMMixer1_2, chorusMix, lfo, fb, lineV);
               
        
        //Frequency modulation to change by half steps, and change key of scale               
        frequencyAdj = frequency;
        frequencyAdj = (4095.00-frequencyAdj)/512;
        frequency = frequencyAdj;
        float mod = pow(1.059463094359,frequency);
        
        //CHANGE OCTAVES
        float octave = 1;
        octaveUp = digitalRead(PM_6);
        octaveDown = digitalRead(PA_7);
        
        if (prevOctU == 0 && prevOctD == 0){
            if (octaveUp == 1){
              flag++;
              prevOctU = 1;
              //octave = pow(1.059463094359,12.00);
            }
            else if (octaveDown == 1){
              prevOctD = 1;
              //octave = pow(1.059463094359,-12.00);
              flag--;
            }
        }
        else if (prevOctU == 1 && octaveUp == 0){
          prevOctU = 0;
        }
        else if (prevOctD == 1 && octaveDown == 0){
          prevOctD = 0;
        }
        
         octave = pow(pow(1.059463094359,12.00),flag);
        
        
        //Output waveforms based on level of touch
        
        //C tones        
        if(forceResistC < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothC, freqC*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinC, freqC*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleC, freqC*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareC, freqC*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothC, freqC*mod*octave, true);  
           sine_source(DEVICE_ADDR_7bit, SinC, freqC*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareC, freqC*mod*octave, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleC, freqC*mod*octave, true);
        }
        
        //D tones        
        if(forceResistD < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothD, freqD*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinD, freqD*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareD, freqD*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleD, freqD*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothD, freqD*mod*octave, true);  
           sine_source(DEVICE_ADDR_7bit, SinD, freqD*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareD, freqD*mod*octave, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleD, freqD*mod*octave, true);
        }
        
        //E tones        
        if(forceResistE < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothE, freqE*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinE, freqE*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareE, freqE*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleE, freqE*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothE, freqE*mod*octave, true); 
           sine_source(DEVICE_ADDR_7bit, SinE, freqE*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareE, freqE*mod*octave, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleE, freqE*mod*octave, true);
        }
        
        //F tones
        if(forceResistF < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothF, freqF*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinF, freqF*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareF, freqF*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleF, freqF*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothF, freqF*mod*octave, true); 
           sine_source(DEVICE_ADDR_7bit, SinF, freqF*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareF, freqF*mod*octave, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleF, freqF*mod*octave, true);
        }
        
        //G tones
        if(forceResistG < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothG, freqG*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinG, freqG*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareG, freqG*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleG, freqG*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothG, freqG*mod*octave, true);
           sine_source(DEVICE_ADDR_7bit, SinG, freqG*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareG, freqG*mod*octave, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleG, freqG*mod*octave, true);
        }
        
        //A tones
        if(forceResistA < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothA, freqA*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinA, freqA*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareA, freqA*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleA, freqA*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothA, freqA*mod*octave, true); 
           sine_source(DEVICE_ADDR_7bit, SinA, freqA*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareA, freqA*mod*octave, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleA, freqA*mod*octave, true);
        }
        
        //B tones
        if(forceResistB < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothB, freqB*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinB, freqB*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareB, freqB*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleB, freqB*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothB, freqB*mod*octave, true);  
           sine_source(DEVICE_ADDR_7bit, SinB, freqB*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareB, freqB*mod*octave, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleB, freqB*mod*octave, true);
        }
        
        //C5 tones
        if(forceResistC5 < 512){
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothC5, freqC5*mod*octave, false);
           sine_source(DEVICE_ADDR_7bit, SinC5, freqC5*mod*octave, false);
           square_source(DEVICE_ADDR_7bit, SquareC5, freqC5*mod*octave, false);
           triangle_source(DEVICE_ADDR_7bit, TriangleC5, freqC5*mod*octave, false);
        }
        else
        {
           sawtooth_source(DEVICE_ADDR_7bit, SawtoothC5, freqC5*mod*octave, true); 
           sine_source(DEVICE_ADDR_7bit, SinC5, freqC5*mod*octave, true);
           square_source(DEVICE_ADDR_7bit, SquareC5, freqC5*mod, true);
           triangle_source(DEVICE_ADDR_7bit, TriangleC5, freqC5*mod*octave, true);
        }
        
        
        
        //Change amplitude of output pitchbased on force level inputted
        
        if (forceResistC < 1024){
          cVolumeAdj = 0.25;
        }
        else if (forceResistC >= 1024 && forceResistC < 1536){
          cVolumeAdj = 0.75;
        }
        else if (forceResistC >= 1536 && forceResistC < 2048){
          cVolumeAdj = 0.85;
        }
        else if (forceResistC >= 2048){
          cVolumeAdj = 1.00;
        }
        
        
        if (forceResistD < 1024){
          dVolumeAdj = 0.25;
        }
        else if (forceResistD >= 1024 && forceResistD < 1536){
          dVolumeAdj = 0.75;
        }
        else if (forceResistD >= 1536 && forceResistD < 2048){
          dVolumeAdj = 0.85;
        }
        else if (forceResistD >= 2048){
          dVolumeAdj = 1.00;
        }
        
        if (forceResistE < 1024){
          eVolumeAdj = 0.25;
        }
        else if (forceResistE >= 1024 && forceResistE < 1536){
          eVolumeAdj = 0.75;
        }
        else if (forceResistE >= 1536 && forceResistE < 2048){
          eVolumeAdj = 0.85;
        }
        else if (forceResistE >= 2048){
          eVolumeAdj = 1.00;
        }
        
        if (forceResistF < 1024){
          fVolumeAdj = 0.25;
        }
        else if (forceResistF >= 1024 && forceResistF < 1536){
          fVolumeAdj = 0.75;
        }
        else if (forceResistF >= 1536 && forceResistF < 2048){
          fVolumeAdj = 0.85;
        }
        else if (forceResistF >= 2048){
          fVolumeAdj = 1.00;
        }
        
        if (forceResistG < 1024){
          gVolumeAdj = 0.25;
        }
        else if (forceResistG >= 1024 && forceResistG < 1536){
          gVolumeAdj = 0.75;
        }
        else if (forceResistG >= 1536 && forceResistG < 2048){
          gVolumeAdj = 0.85;
        }
        else if (forceResistG >= 2048){
          gVolumeAdj = 1.00;
        }
        
        if (forceResistA < 1024){
          aVolumeAdj = 0.25;
        }
        else if (forceResistA >= 1024 && forceResistA < 1536){
          aVolumeAdj = 0.75;
        }
        else if (forceResistA >= 1536 && forceResistA < 2048){
          aVolumeAdj = 0.85;
        }
        else if (forceResistA >= 2048){
          aVolumeAdj = 1.00;
        }
        
        if (forceResistB < 700){
          bVolumeAdj = 1.00;
        }
        else if (forceResistB >= 700 && forceResistB < 2048){
          bVolumeAdj = 1.00;
        }
        else if (forceResistB >= 2048){
          bVolumeAdj = 1.00;
        }
        
        if (forceResistC5 < 1024){
          c5VolumeAdj = 0.25;
        }
        else if (forceResistC5 >= 1024 && forceResistC5 < 1536){
          c5VolumeAdj = 0.75;
        }
        else if (forceResistC5 >= 1536 && forceResistC5 < 2048){
          c5VolumeAdj = 0.85;
        }
        else if (forceResistC5 >= 2048){
          c5VolumeAdj = 1.00;
        }
        
        //Write to output levels for each pitch
        Mixer8ChannelVolume(DEVICE_ADDR_7bit, SawMixer, SinMixer, SquareMixer, TriMixer, cVolumeAdj, dVolumeAdj, eVolumeAdj, fVolumeAdj, gVolumeAdj, aVolumeAdj, bVolumeAdj, c5VolumeAdj); //change volume of waveform mixer channels

        
} 

