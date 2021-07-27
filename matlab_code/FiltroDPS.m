%% Prueba filtro DSP / arduino due
clc; clear all; close all;
cd('D:\INGENIERIA\año 5\1ercuatrimestre\DSP - PROM 9\EJERCICIOSMATLAB');
[x,f]=audioread('guitarsample.wav');
x=x(:,1);
sound(x,f);
Fs=100000;
Ts=1/Fs;
t=linspace(-0.1,0.1,length(x));
subplot(2,1,1);
plot(t,x);title('señal de audio');
X=fftshift(fft(x,length(x))/length(x));
F=linspace(-Fs/2,Fs/2,length(X));
subplot(2,1,2);
plot(F,abs(X));title('espectro de la señal');xlim([-10000 10000]);
%%  agrego Flanger
delay=15; % tiempo en milisegundos despues de la señal original donde va ser sumada una nueva señal
rango=12; % amplitud de la señal sumada
fbarrido=0.3125; % "velocidad" de la señal sumada.
% filtro flanger por medio de ecuacion en diferencias, esta ecuacion tiene
% 3 componentes con los que se modula el efecto de phaser/flanger/chorus
for i=1:(length(x)-rango-delay)
    y(i)=x(i) + x(i + delay + round(rango*sin(2*pi*i*(fbarrido/f))));
end
% lo siguiente es  completar y con ceros para poder plotearlo, ya que los 
% vectores t e y me quedan de distinta longitud
LY=length(y);
for i=1:length(t)-LY
    y(i+LY)=0;
end
 % grafico
figure(3)
subplot(2,1,1);
plot(t,y);title('señal de audio filtrada con flanger');
Y=fftshift(fft(y,length(y))/length(y));
F1=linspace(-Fs/2,Fs/2,length(Y));
subplot(2,1,2);
plot(F1,abs(Y));title('espectro de la señal filtrada con flanger');xlim([-10000 10000]);
sound(y,f)
%% chorus
delay=20;% tiempo en milisegundos despues de la señal original donde va ser sumada una nueva señal
rango=50;
fbarrido=0.4;
%filtro flanger por medio de ecuacion en diferencias, esta ecuacion tiene
% 3 componentes con los que se modula el efecto de phaser/flanger/chorus
for i=1:(length(x)-rango-delay)
    y(i)=x(i) + x(i + delay + round(rango*sin(2*pi*i*(fbarrido/f))));
end
% lo siguiente es  completar y con ceros para poder plotearlo, ya que los 
% vectores t e y me quedan de distinta longitud
LY=length(y);
for i=1:length(t)-LY
    y(i+LY)=0;
end
 % grafico
figure(3)
subplot(2,1,1);
plot(t,y);title('señal de audio filtrarerrrrreeeeeeeeeeeeeeeeeefdqda con flanger');
Y=fftshift(fft(y,length(y))/length(y));
F1=linspace(-Fs/2,Fs/2,length(Y));
subplot(2,1,2);
plot(F1,abs(Y));title('espectro de la señal filtrada con flanger');xlim([-10000 10000]);
sound(y,f)
%% chorus 2.0
delay1=30;% tiempo en milisegundos despues de la señal original donde va ser sumada una nueva señal
rango1=50;
fbarrido1=2;
delay2=15; % tiempo en milisegundos despues de la señal original donde va ser sumada una nueva señal
rango2=14; % amplitud de la señal sumada
fbarrido2=0.3125;
%filtro flanger por medio de ecuacion en diferencias, esta ecuacion tiene
% 3 componentes con los que se modula el efecto de phaser/flanger/chorus
for i=1:(length(x)-rango2-delay1)
    y(i)=x(i) + x(i + delay1 + round(rango1*sin(2*pi*i*(fbarrido1/f))))+ x(i + delay2 + round(rango2*sin(2*pi*i*(fbarrido2/f))));
end
% lo siguiente es  completar y con ceros para poder plotearlo, ya que los 
% vectores t e y me quedan de distinta longitud
LY=length(y);
for i=1:length(t)-LY
    y(i+LY)=0;
end
 % grafico
figure(3)
subplot(2,1,1);
plot(t,y);title('señal de audio filtrada con flanger');
Y=fftshift(fft(y,length(y))/length(y));
F1=linspace(-Fs/2,Fs/2,length(Y));
subplot(2,1,2);
plot(F1,abs(Y));title('espectro de la señal filtrada con flanger');xlim([-10000 10000]);
sound(y,f)
%% OTRO FLANGER
lfo_freq = 1/3; % LFO Freq (Hz)
lfo_amp = 0.004; % LFO Amp (sec)
lfo = 2+sawtooth(2*pi*lfo_freq*(1:length(x))/f,0.5); % Generate triangle wave
index = round((1:length(x))-f*lfo_amp*lfo); % Read-out index
index(index<1) = 1; % Clip delay
index(index>length(x)) = length(x);
y=x; % Input Signal

for j=1:length(x); % For each sample
y(j) = y(j)+x(index(j)); % Add delayed signal
end
 % grafico
figure(3)
subplot(2,1,1);
plot(t,y);title('señal de audio filtrada con flanger');
Y=fftshift(fft(y,length(y))/length(y));
F1=linspace(-Fs/2,Fs/2,length(Y));
subplot(2,1,2);
plot(F1,abs(Y));title('espectro de la señal filtrada con flanger');xlim([-10000 10000]);
sound(y,f)

%% distortion
a=0.9;
k = 2*a/(1-a);
y= (1+k)*(x)./(1+k*abs(x));
figure(3)
subplot(2,1,1);
plot(t,y);title('señal de audio filtrada con distorsion');
Y=fftshift(fft(y,length(y))/length(y));
F1=linspace(-Fs/2,Fs/2,length(Y));
subplot(2,1,2);
plot(F1,abs(Y));title('espectro de la señal filtrada con distorsion');xlim([-10000 10000]);
sound(y,f)
%% Filtro FIR pasabajo
pasabajo=[0.00720845348367704,0.00927912086341559,0.0152549779154740,0.0245915630709705,0.0363999242637434,0.0495344523495799,0.0627075376059401,0.0746193693865725,0.0840895651631092,0.0901770691219197,0.0922759335511966,0.0901770691219197,0.0840895651631092,0.0746193693865725,0.0627075376059401,0.0495344523495799,0.0363999242637434,0.0245915630709705,0.0152549779154740,0.00927912086341559,0.00720845348367704];
y=filter(pasabajo,1,x);
figure(3)
subplot(2,1,1);
plot(t,y);title('señal de audio filtrada con pasabajo');
Y=fftshift(fft(y,length(y))/length(y));
F1=linspace(-Fs/2,Fs/2,length(Y));
subplot(2,1,2);
plot(F1,abs(Y));title('espectro de la señal filtrada con pasabajo');xlim([-10000 10000]);
sound(y,f)
%% Filtro FIR pasaalto
pasaalto=[0.00230044342036761,0.00234463960716355,0.00182733736931652,-0.00182292548305452,-0.0114868354545796,-0.0289405275423659,-0.0536317553670062,-0.0822325278453450,-0.109266727504802,-0.128669350637162,0.861873691271880,-0.128669350637162,-0.109266727504802,-0.0822325278453450,-0.0536317553670062,-0.0289405275423659,-0.0114868354545796,-0.00182292548305452,0.00182733736931652,0.00234463960716355,0.00230044342036761];
y=filter(pasaalto,1,x);
figure(4)
subplot(2,1,1);
plot(t,y);title('señal de audio filtrada con pasaalto');
Y=fftshift(fft(y,length(y))/length(y));
F1=linspace(-Fs/2,Fs/2,length(Y));
subplot(2,1,2);
plot(F1,abs(Y));title('espectro de la señal filtrada con pasaalto');xlim([-10000 10000]);
sound(y,f)
%%
figure(5)
freqz(pasaalto,1)
figure(6)
freqz(pasabajo,1)