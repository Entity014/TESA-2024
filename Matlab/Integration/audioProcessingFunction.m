function audioProcessingFunction(audioData, Fs)
    frameLength = 256;

    bufferLength = round(5.0 * Fs); 
    audioBuffer = zeros(1, bufferLength);
    bufferPosition = 1;
    audioPositionSinceSync = 1;

    maxPeak = 0.0;
    startThreshold = db2mag(-30);  

    shortWinLength = round(0.3 * Fs);
    longWinLength = round(2.0 * Fs);

    while audioPositionSinceSync <= length(audioData)
        signal = audioData(audioPositionSinceSync : audioPositionSinceSync + frameLength - 1);
        if max(signal) > startThreshold
            maxPeak = max(signal);
            break;
        end
        audioPositionSinceSync = audioPositionSinceSync + frameLength;
    end

    while audioPositionSinceSync <= length(audioData)
        signal = audioData(audioPositionSinceSync : audioPositionSinceSync + frameLength - 1);

        signalPeak = max(abs(signal));
        if signalPeak > maxPeak
            maxPeak = signalPeak;
        end
        signal = signal / maxPeak;
        
        frameLength = length(signal);
        emptyLength = bufferLength - bufferPosition + 1;
        if emptyLength < frameLength
            moveOffset = frameLength - emptyLength;
            audioBuffer(1:bufferPosition - moveOffset - 1) = audioBuffer(moveOffset + 1 : bufferPosition - 1);
            bufferPosition = bufferPosition - moveOffset;
        end
        audioBuffer(bufferPosition : bufferPosition + frameLength - 1) = signal;
        bufferPosition = bufferPosition + frameLength;
        audioPositionSinceSync = audioPositionSinceSync + frameLength;

        if audioPositionSinceSync + frameLength - 1 > length(audioData)
            break; 
        end
        
        env1 = movingRMS(signal, shortWinLength);  
        env2 = movingRMS(signal, longWinLength);  
        envDiff = env1 + env2;
        threshold = 0.9;
        segments = envDiff > threshold;
        segmentIndices = find(segments);


        if ~isempty(segmentIndices)
            if max(segmentIndices) <= length(audioBuffer)
                segment = audioBuffer(segmentIndices(1):segmentIndices(end));
                if length(segment) >= frameLength
                     fprintf("Audio Position: %.2f\n", audioPositionSinceSync);
                end
            else
                warning('Segment indices exceed buffer length');
            end
        end
    end
end

function rmsValue = movingRMS(signal, windowLength)
    squaredSignal = signal.^2;  
    rmsValue = sqrt(movmean(squaredSignal, windowLength));  
end
