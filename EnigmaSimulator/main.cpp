#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "EnigmaSimulator.h"

int main() {
    EnigmaSimulator enigma;
    std::cout << "==========================================\n";
    std::cout << "Welcome to Enigma Simulator Application !!\n";
    std::cout << "==========================================\n\n";

    // Ask user for reflector choice
    int reflectorChoice;
    std::cout << "Choose Reflector (1 or 2): ";
    std::cin >> reflectorChoice;
    enigma.SetReflector(reflectorChoice);

    // Flush the newline character from the input buffer
    std::cin.ignore();  // This ensures the next getline works properly

    std::vector<int> customRotors;
    std::string rotorInput;
    bool validInput = false;

    while (!validInput) {
        std::cout << "Enter rotor shifts (integers from 1 to 5, separated by spaces, e.g., 1 2 3): ";
        std::getline(std::cin, rotorInput);
        std::istringstream input_stream(rotorInput);
        customRotors.clear();
        int rotor;
        validInput = true;

        while (input_stream >> rotor) {
            if (rotor < 1 || rotor > 5) {
                std::cout << "Invalid rotor number " << rotor << ". Please enter rotors between 1 and 5.\n\n";
                validInput = false;
                break;
            }
            customRotors.push_back(rotor - 1);
        }

        if (customRotors.empty()) {
            std::cout << "Please enter at least one valid rotor number.\n\n";
            validInput = false;
        }
    }

    enigma.SetUpRotors(customRotors);

    std::vector<int> initialPositions;
    std::string positionInput;
    bool validPositions = false;

    while (!validPositions) {
        std::cout << "Enter initial positions for each rotor (A-Z, separated by spaces, e.g., A B C): ";
        std::getline(std::cin, positionInput);
        std::istringstream pos_stream(positionInput);
        initialPositions.clear();
        char positionChar;
        validPositions = true;

        while (pos_stream >> positionChar) {
            if (!std::isalpha(positionChar) || toupper(positionChar) < 'A' || toupper(positionChar) > 'Z') {
                std::cout << "Invalid position " << positionChar << ". Please enter letters between A and Z.\n\n";
                validPositions = false;
                break;
            }
            initialPositions.push_back(toupper(positionChar) - 'A');
        }

        if (initialPositions.size() != customRotors.size()) {
            std::cout << "Error: Number of positions does not match number of rotors. Please try again.\n\n";
            validPositions = false;
        }
    }

    enigma.SetInitialPositions(initialPositions);

    std::vector<int> ringSettings;
    std::string ringInput;
    bool validRingSettings = false;

    while (!validRingSettings) {
        std::cout << "Enter ring settings for each rotor (integers from 0-25, corresponding to letters A-Z, separated by spaces, e.g., 1 2 3): ";
        std::getline(std::cin, ringInput);
        std::istringstream ring_stream(ringInput);
        ringSettings.clear();
        int ring;
        validRingSettings = true;

        while (ring_stream >> ring) {
            if (ring < 0 || ring > 25) {
                std::cout << "Invalid ring setting " << ring << ". Please enter numbers between 0 and 25.\n\n";
                validRingSettings = false;
                break;
            }
            ringSettings.push_back(ring);
        }

        if (ringSettings.size() != customRotors.size()) {
            std::cout << "Error: Number of ring settings does not match number of rotors. Please try again.\n\n";
            validRingSettings = false;
        }
    }

    enigma.SetRingSettings(ringSettings);

    std::vector<std::pair<char, char>> plugboardSettings;
    std::string plugInput;
    std::cout << "Enter up to 13 plugboard pairs (e.g., A-B C-D), or press Enter to skip: ";
    std::getline(std::cin, plugInput);

    if (!plugInput.empty()) {
        std::istringstream plugStream(plugInput);
        char plug1, dash, plug2;

        while (plugStream >> plug1 >> dash >> plug2) {
            if (dash == '-' && std::isalpha(plug1) && std::isalpha(plug2)) {
                if (plugboardSettings.size() >= 13) {
                    std::cout << "Maximum of 13 plugboard pairs reached. Ignoring additional pairs.\n";
                    break;
                }
                plugboardSettings.emplace_back(toupper(plug1), toupper(plug2));
            }
            else {
                std::cout << "Invalid format. Use format like A-B. Skipping incorrect pairs.\n";
            }
        }
    }
    else {
        std::cout << "No plugboard pairs entered. Proceeding without plugboard swaps.\n";
    }

    // Print all the plugboard pairs that the user entered
    if (!plugboardSettings.empty()) {
        std::cout << "\nPlugboard pairs entered: \n";
        for (const auto& pair : plugboardSettings) {
            std::cout << pair.first << '-' << pair.second << " ";
        }
        std::cout << std::endl;
    }

    // Set plugboard settings for Enigma
    enigma.SetPlugboard(plugboardSettings);

    std::cout << "\nEnter the message to encrypt (press Enter without typing anything when you're done):\n";
    std::string inputMessage, line;
    while (true) {
        std::getline(std::cin, line);
        if (line.empty()) break;
        inputMessage += line + "\n";
    }
    std::cout << std::endl;

    // Save the initial positions for resetting before decryption
    std::vector<int> initialPositionsCopy = initialPositions;

    std::cout << "Original Message:\n" << inputMessage << "\n";
    std::string encryptedMessage = enigma.Encrypt(inputMessage);
    std::cout << "Encrypted Message:\n" << encryptedMessage << "\n\n";

    // Reset the initial positions before decryption
    enigma.SetInitialPositions(initialPositionsCopy);

    std::string decryptedMessage = enigma.Decrypt(encryptedMessage);
    std::cout << "Decrypted Message:\n" << decryptedMessage << "\n";

    return 0;
}
