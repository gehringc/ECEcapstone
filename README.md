# 2015 Capstone Project - Digital 4-Osc Polyphonic Synthesizer

This group project used a TI Tiva C microcontroller, programmed on Energia (TI's open source platform resembling Arduino) to control an Analog Devices DSP development board in order to generate synthesized sounds. My primary responsibility was to design the DSP signal flow and integrate the DSP backend with the microcontroller.

Here is an overview of the project contents:
[EnergiaSrc](https://github.com/gehringc/ECEcapstone/tree/master/EnergiaSrc) contains the Arduino/Energia program uploaded to the TI TivaC
[DSP Libraries](https://github.com/gehringc/ECEcapstone/tree/master/DSP%20Libraries) contains the API provided by the DSP board manufacturer to interface DSP functions with the TivaC
[SigmaStudio Files](https://github.com/gehringc/ECEcapstone/tree/master/SigmaStudio%20Files) contains the actual project file and it's outputs from the Analog Devices DSP design tool. The hex file outputs were integrated in the EnergiaSrc header file to program the DSP after TivaC flashing.
[Documentation](https://github.com/gehringc/ECEcapstone/tree/master/Documentation) provides reference images for the project. Sorry for the lack of documentation. It seems the group has removed the formal report, I will try to fix this. 
