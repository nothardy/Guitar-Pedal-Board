# Guitar-Pedal-Board

Guitar Pedal Board in C++, implemented on MATLAB and Arduino due.

# How it works?

- First code is a MATLAB script which samples a clean guitar .wav file and plots its time function and frequency spectrum. Then each scrip section implements a filter that mimics a guitar pedal effect, such as: flangers, overdrive, low-pass and high-pass; also plotting the result time function and frequency spectrum.

- Second code goes by the same idea as the first one, but adapted for an Arduino Due board in C++. For this arduino board, a shield board was designed and solder so that the user can adjust volume and connect a guitar and also an amplifier. Two versions of this code were uploaded, first one including a led display; the second one, a more clean and newer version already debugged and without the display.

All of these guitar effects were calculated using the discrete Fourier Transform and Sampling Theory, learnt in a Digital Signal Processing course at Uncoma. Each effect is represented by a impulse response that is applyed to the input signal received at the Arduino.

# Can I try it at home?

Sure! Although, you may need to implement/solder your own shield board. Kinda messy huh? Well, you still can try the MATLAB version. It runs on MATLAB R2016a and higher!

- First step is to have MATLAB installed and download .m script and .wav file all together in a same folder. Then run first section of the code by stepping into it and pressing Ctrl + Enter. Math plot windows will show and you will hear wav file playing, it's a clean guitar sample.

- Second step is to choose any filter, step into it and run Ctrl + Enter, you will hear the same guitar sample but with effects now!
