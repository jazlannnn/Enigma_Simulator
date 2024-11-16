#ifndef ENIGMA_SIMULATOR_H
#define ENIGMA_SIMULATOR_H

#include <string>
#include <vector>
#include <unordered_map>

class EnigmaSimulator {
public:
    EnigmaSimulator();
    void SetUpRotors(const std::vector<int>& rotors);
    void SetInitialPositions(const std::vector<int>& positions);
    void SetRingSettings(const std::vector<int>& ringSettings);
    void SetPlugboard(const std::vector<std::pair<char, char>>& plugboardSettings);
    void SetReflector(int reflectorChoice);
    std::string Encrypt(const std::string& message);
    std::string Decrypt(const std::string& message);

private:
    std::vector<int> rotors;
    std::vector<int> positions;
    std::vector<int> ringSettings;
    std::vector<int> notchPositions;
    std::unordered_map<char, char> plugboard;
    int reflector;

    char ApplyPlugboard(char c);
    char RotateCharacterForEncryption(char c, int rotorIndex);
    char RotateCharacterForDecryption(char c, int rotorIndex);
    void StepRotors();
    char ReflectCharacter(char c);
};

#endif
