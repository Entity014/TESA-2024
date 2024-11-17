function spectrum = sound_freq(X)
arguments
    X(1,4096) double 
end
    Y = fft(X);
    mag = abs(Y);
    spectrum = mag(1:2048);
end