function ema = calcEMA(data, N) %#codegen
    arguments
        data(1,100) double;
        N(1,1) uint32;
    end
    % https://www.investopedia.com/terms/e/ema.asp
    alpha = 2.0 / (N + 1); % Calculate the smoothing factor
    ema = zeros(size(data)); % Preallocate EMA array
    ema(1) = data(1); % Initialize the first EMA value

    % Calculate EMA using vector operations
    for t = 2:length(data)
        ema(t) = alpha * data(t) + (1 - alpha) * ema(t - 1);
    end
end