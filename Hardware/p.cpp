#include <iostream>
#include <sndfile.h>

int main() {
    // Path to the .wav file you want to open
    const char* filename = "recordingmadi3.wav";
    
    // Open the .wav file for reading
    SF_INFO sf_info;
    SNDFILE* sndfile = sf_open(filename, SFM_READ, &sf_info);

    // Check if file was opened successfully
    if (sndfile == nullptr) {
        std::cerr << "Error opening file: " << sf_strerror(sndfile) << std::endl;
        return 1;
    }

    // Print some information about the .wav file
    std::cout << "Sample rate: " << sf_info.samplerate << std::endl;
    std::cout << "Channels: " << sf_info.channels << std::endl;
    std::cout << "Frames: " << sf_info.frames << std::endl;
    std::cout << "Format: " << sf_info.format << std::endl;

    // You can now read the audio data from the file if you want.
    // For example, we can read the samples into a buffer:
    int num_samples = sf_info.frames * sf_info.channels;
    float* buffer = new float[num_samples];

    sf_count_t num_frames_read = sf_readf_float(sndfile, buffer, sf_info.frames);
    if (num_frames_read < sf_info.frames) {
        std::cerr << "Warning: not all frames were read!" << std::endl;
    }

    // Do something with the data...
    // For example, print the first 10 samples
    std::cout << "First 10 samples (if available):" << std::endl;
    for (int i = 0; i < 10 && i < num_samples; ++i) {
        std::cout << buffer[i] << std::endl;
    }

    // Cleanup
    delete[] buffer;
    sf_close(sndfile);

    return 0;
}
