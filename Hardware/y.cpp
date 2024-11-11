#include <SFML/Audio.hpp>
#include <iostream>

int main() {
    // Path to the .wav file you want to play
    const char* filename = "recordingmadi3.wav";
    
    // Create a SoundBuffer to hold the audio data from the .wav file
    sf::SoundBuffer buffer;

    // Load the .wav file into the buffer
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Error loading file: " << filename << std::endl;
        return -1;
    }

    // Create a Sound object to play the audio
    sf::Sound sound;
    sound.setBuffer(buffer);
    
    // Play the sound
    sound.play();
    
    // Wait until the sound finishes playing
    // While the sound is playing, we'll block and do nothing (just wait)
    while (sound.getStatus() == sf::Sound::Playing) {
        // You can do other things here if needed
        // For simplicity, we just wait
    }

    std::cout << "Playback finished!" << std::endl;
    return 0;
}
