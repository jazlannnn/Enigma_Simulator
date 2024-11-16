#include <iostream>
#include "EnigmaSimulator.h"
#include <algorithm>
#include <cctype>
#include <unordered_map>

EnigmaSimulator::EnigmaSimulator() {
    rotors = { 0, 1, 2 };
    positions = { 0, 0, 0 };
    ringSettings = { 0, 0, 0 };
    notchPositions = { 16, 4, 21 };
    reflector = 1;
}

void EnigmaSimulator::SetUpRotors(const std::vector<int>& customRotors) {
    rotors = customRotors;
}

void EnigmaSimulator::SetInitialPositions(const std::vector<int>& initialPositions) {
    positions = initialPositions;
}

void EnigmaSimulator::SetRingSettings(const std::vector<int>& ringSettingsInput) {
    ringSettings = ringSettingsInput;
}

void EnigmaSimulator::SetPlugboard(const std::vector<std::pair<char, char>>& plugboardSettings) {
    plugboard.clear();
    for (const auto& pair : plugboardSettings) {
        char first = std::toupper(pair.first);
        char second = std::toupper(pair.second);
        plugboard[first] = second;
        plugboard[second] = first;
    }
}

void EnigmaSimulator::SetReflector(int reflectorChoice) {
    reflector = (reflectorChoice == 1 || reflectorChoice == 2) ? reflectorChoice : 1;
}

char EnigmaSimulator::ApplyPlugboard(char c) {
    return plugboard.count(c) ? plugboard[c] : c;
}

void EnigmaSimulator::StepRotors() {
    positions[0] = (positions[0] + 1) % 26;

    if (positions[0] == notchPositions[0]) {
        positions[1] = (positions[1] + 1) % 26;
        if (positions[1] == notchPositions[1]) {
            positions[2] = (positions[2] + 1) % 26;
        }
    }
}

char EnigmaSimulator::RotateCharacterForEncryption(char c, int rotorIndex) {
    int shift = (positions[rotorIndex] + ringSettings[rotorIndex]) % 26;
    c = ((c - 'A' + shift + 26) % 26) + 'A';
    return c;
}

char EnigmaSimulator::RotateCharacterForDecryption(char c, int rotorIndex) {
    int shift = (positions[rotorIndex] + ringSettings[rotorIndex]) % 26;
    c = ((c - 'A' - shift + 26) % 26) + 'A';
    return c;
}

char EnigmaSimulator::ReflectCharacter(char c) {
    static const std::unordered_map<char, char> reflectorI = {
        {'A', 'Y'}, {'B', 'R'}, {'C', 'U'}, {'D', 'H'}, {'E', 'Q'}, {'F', 'S'}, {'G', 'L'}, {'H', 'D'},
        {'I', 'P'}, {'J', 'X'}, {'K', 'N'}, {'L', 'G'}, {'M', 'O'}, {'N', 'K'}, {'O', 'M'}, {'P', 'I'},
        {'Q', 'E'}, {'R', 'B'}, {'S', 'F'}, {'T', 'Z'}, {'U', 'C'}, {'V', 'W'}, {'W', 'V'}, {'X', 'J'},
        {'Y', 'A'}, {'Z', 'T'}
    };
    static const std::unordered_map<char, char> reflectorII = {
        {'A', 'F'}, {'B', 'W'}, {'C', 'J'}, {'D', 'Z'}, {'E', 'G'}, {'F', 'A'}, {'G', 'E'}, {'H', 'L'},
        {'I', 'Y'}, {'J', 'C'}, {'K', 'R'}, {'L', 'H'}, {'M', 'X'}, {'N', 'O'}, {'O', 'N'}, {'P', 'U'},
        {'Q', 'S'}, {'R', 'K'}, {'S', 'Q'}, {'T', 'P'}, {'U', 'V'}, {'V', 'U'}, {'W', 'B'}, {'X', 'M'},
        {'Y', 'I'}, {'Z', 'D'}
    };

    return (reflector == 1) ? reflectorI.at(c) : reflectorII.at(c);
}

std::string EnigmaSimulator::Encrypt(const std::string& message) {
    std::string encryptedMessage = message;

    for (char& c : encryptedMessage) {
        if (std::isalpha(c)) {
            bool isLower = std::islower(c);
            c = std::toupper(c);

            c = ApplyPlugboard(c);

            for (int i = 0; i < rotors.size(); ++i) {
                c = RotateCharacterForEncryption(c, i);
            }

            c = ReflectCharacter(c);

            for (int i = rotors.size() - 1; i >= 0; --i) {
                c = RotateCharacterForDecryption(c, i);
            }

            c = ApplyPlugboard(c);

            if (isLower) c = std::tolower(c);
        }

        StepRotors();
    }
    return encryptedMessage;
}

std::string EnigmaSimulator::Decrypt(const std::string& message) {
    return Encrypt(message);
}
