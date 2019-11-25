#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <deque>
#include <limits>

template <class Iterator>
void printContainer(Iterator begin, Iterator end) {
  for (auto it = begin; it < end; ++it) {
    std::cout << *it;
  }
}

std::deque<bool> fromDecimalToBinary(long long SomeNumber, size_t NumberOfBits);
int fromBinaryToDecimal(std::deque<bool> someVector);
std::deque<bool> supplementaryCode(long long SomeNumber, size_t NumberOfBits);
void informationToOutputAboutMov (std::string commandToOutput, std::string registrToOutput, long long numberToOutput,
                          std::deque<bool> firstVector, std::deque<bool> secondVector, std::deque<bool> thirdVector,
                          bool lastSign, int commandsCounter, int tactsCounter);
void informationToOutputAboutMod(std::string commandToOutput, std::string registrToOutput, std::string secondRegistr, std::string thirdRegistr,
                                 std::deque<bool> firstVector, std::deque<bool> secondVector, std::deque<bool> thirdVector,
                                 bool lastSign, int commandsCounter, int tactsCounter);


int main() {
  std::string pathToFile = "/home/jaytaddy/Programming/2nd_course_KNU/LabolatornaRobota/AOS2/ImitationOfTheCPU/commands.txt";
  std::ifstream commandsFile(pathToFile, std::ifstream::in | std::ifstream::out | std::ifstream::app);
  std::vector<std::pair<std::pair<std::string, std::string>, int>> commandsVector;

  if (!commandsFile.is_open()) {
    std::cout << "Cannot open a file, check if it's name is correct!" << std::endl;
  } else {
    std::string command = "";
    std::string registr = "";
    std::deque<bool> firstRegister = fromDecimalToBinary(15, 24);
    std::deque<bool> secondRegister = fromDecimalToBinary(62, 24);
    std::deque<bool> thirdRegister = fromDecimalToBinary(-124, 24);
    bool lastSign = 0;
    int commandsCounter = 1;

    while(!commandsFile.eof()) {
      commandsFile >> command;
      int tactsCounter = 1;
      long long number = 0;
      if (command == "mov" || command == "Mov" || command == "MOV") {
        commandsFile >> registr >> number;
        if (number > std::numeric_limits<int>::max()) {
          std::cout << "In case that you entered number that can't be assign to int_type\n"
                       "Your number will be swapped to 0!" << std::endl;
          number = 0;
        }

        informationToOutputAboutMov(command, registr, number, firstRegister, secondRegister, thirdRegister,
                            lastSign, commandsCounter, tactsCounter);

        if (registr == "R1,") {
          if (number < 0) {
            firstRegister = fromDecimalToBinary(number, 24);
            lastSign = 1;
            ++tactsCounter;
          } else {
            firstRegister = fromDecimalToBinary(number, 24);
            lastSign = 0;
            ++tactsCounter;
          }
        } else if (registr == "R2,") {
          if (number < 0) {
            secondRegister = fromDecimalToBinary(number, 24);
            lastSign = 1;
            ++tactsCounter;
          } else {
            secondRegister = fromDecimalToBinary(number, 24);
            lastSign = 0;
            ++tactsCounter;
          }
        } else if (registr == "R3,") {
          if (number < 0) {
            thirdRegister = fromDecimalToBinary(number, 24);
            lastSign = 1;
            ++tactsCounter;
          } else {
            thirdRegister = fromDecimalToBinary(number, 24);
            lastSign = 0;
            ++tactsCounter;
          }
        }

        informationToOutputAboutMov(command, registr, number, firstRegister, secondRegister, thirdRegister,
                            lastSign, commandsCounter, tactsCounter);
        ++commandsCounter;
      } else if (command == "mod" || command == "Mod" || command == "MOD") {
        std::string secondRegistr;
        std::string thirdRegistr;
        commandsFile >> registr >> secondRegistr >> thirdRegistr;

        informationToOutputAboutMod(command, registr, secondRegistr, thirdRegistr, firstRegister, secondRegister, thirdRegister,
                            lastSign, commandsCounter, tactsCounter);

        if ((registr == "R1," || registr == "R2," || registr == "R3,") &&
            (secondRegistr == "R2," || secondRegistr == "R1," || secondRegistr == "R3,") &&
            (thirdRegistr == "R3," || thirdRegistr == "R2," || thirdRegistr == "R1,")) {
          int firstNumber = fromBinaryToDecimal(firstRegister);
          int secondNumber = fromBinaryToDecimal(secondRegister);
          int thirdNumber = fromBinaryToDecimal(thirdRegister);
          if (firstNumber < 0 || secondNumber < 0 || thirdNumber < 0) {
            lastSign = 1;
            ++tactsCounter;
            int result = firstNumber % secondNumber % thirdNumber;
            if (registr == "R1,") {
              firstRegister = fromDecimalToBinary(result, 24);
            } else if (registr == "R2,") {
              secondRegister = fromDecimalToBinary(result, 24);
            } else if (registr == "R3,") {
              thirdRegister = fromDecimalToBinary(result, 24);
            }
          } else {
            lastSign = 0;
            ++tactsCounter;
            int result = firstNumber % secondNumber % thirdNumber;
            if (registr == "R1,") {
              firstRegister = fromDecimalToBinary(result, 24);
            } else if (registr == "R2,") {
              secondRegister = fromDecimalToBinary(result, 24);
            } else if (registr == "R3,") {
              thirdRegister = fromDecimalToBinary(result, 24);
            }
          }
        }

        informationToOutputAboutMod(command, registr, secondRegistr, thirdRegistr, firstRegister, secondRegister, thirdRegister,
                            lastSign, commandsCounter, tactsCounter);

        ++commandsCounter;
        command.erase();
      }
    }
  }
  commandsFile.close();
}


std::deque<bool> fromDecimalToBinary(long long SomeNumber, size_t NumberOfBits) {
  std::deque<bool> binaryNumberForm;
  if (SomeNumber == 0) {
    for (size_t index = 0; index < NumberOfBits; ++index) {
      binaryNumberForm.push_back(0);
    }
  }
  long long numberToInverse = SomeNumber;
  /*if (numberToInverse < 0) {
    numberToInverse *= -1;
  }*/
  if (SomeNumber > 0) {
    while (numberToInverse > 0) {
      if ((numberToInverse % 2) != 0) {
        binaryNumberForm.push_front(1);
        numberToInverse /= 2;
      } else {
        binaryNumberForm.push_front(0);
        numberToInverse /= 2;
      }
    }
    while (binaryNumberForm.size() < NumberOfBits) {
      binaryNumberForm.push_front(0);
    }
  } else if (SomeNumber < 0) {
    binaryNumberForm = supplementaryCode(SomeNumber, NumberOfBits);
  }
  /*if (SomeNumber < 0) {
    binaryNumberForm.at(0) = 1;
  }*/
  return binaryNumberForm;
}

int fromBinaryToDecimal(std::deque<bool> someBitNumber) {
  int resultNumber = 0;
  if (someBitNumber.at(0) == 1) {
    someBitNumber.at(0) = 0;
    int degreeOfTwo = 1;
    for (auto someIterator = someBitNumber.end() - 1; someIterator > someBitNumber.begin(); --someIterator) {
      int result = *someIterator * degreeOfTwo;
      degreeOfTwo *= 2;
      resultNumber += result;
    }
    resultNumber *= -1;
  } else if (someBitNumber.at(0) == 0){
    int degreeOfTwo = 1;
    for (auto someIterator = someBitNumber.end() - 1; someIterator > someBitNumber.begin(); --someIterator) {
      int result = *someIterator * degreeOfTwo;
      degreeOfTwo *= 2;
      resultNumber += result;
    }
  }
  return resultNumber;
}

std::deque<bool> supplementaryCode(long long SomeNumber, size_t NumberOfBits) {
  SomeNumber *= -1;
  std::deque<bool> binaryNumberForm = fromDecimalToBinary(SomeNumber, NumberOfBits);
  for (auto it = binaryNumberForm.begin(); it < binaryNumberForm.end(); ++it) {
    if (*it == 0) {
      *it = 1;
    } else if (*it == 1) {
      *it = 0;
    }
  }
  binaryNumberForm.at(binaryNumberForm.size() - 1) = 1;
  return  binaryNumberForm;
}

void informationToOutputAboutMov (std::string commandToOutput, std::string registrToOutput, long long numberToOutput,
                          std::deque<bool> firstVector, std::deque<bool> secondVector, std::deque<bool> thirdVector,
                          bool lastSign, int commandsCounter, int tactsCounter) {
  std::cout << "IR" << " " << commandToOutput << " " << registrToOutput << " " << numberToOutput << std::endl;
          std::cout << "R1" << " ";
          printContainer(firstVector.begin(), firstVector.end());
          std::cout << std::endl << "R2" << " ";
          printContainer(secondVector.begin(), secondVector.end());
          std::cout << std::endl << "R3" << " ";
          printContainer(thirdVector.begin(), thirdVector.end());
          std::cout << std::endl << std::endl << "PS" << " ";
          if (lastSign == 0) {
            std::cout << "+" << std::endl;
          } else if (lastSign == 1) {
            std::cout << "-" << std::endl;
          }
          std::cout << "PC" << " " << commandsCounter << std::endl << "TC" << " " << tactsCounter << std::endl;
          std::cout << "..................................." << std::endl;
}

void informationToOutputAboutMod(std::string commandToOutput, std::string registrToOutput, std::string secondRegistr, std::string thirdRegistr,
                                 std::deque<bool> firstVector, std::deque<bool> secondVector, std::deque<bool> thirdVector,
                                 bool lastSign, int commandsCounter, int tactsCounter) {
  std::cout << "IR" << " " << commandToOutput << " " << registrToOutput << " " << secondRegistr << " " << thirdRegistr << std::endl;
          std::cout << "R1" << " ";
          printContainer(firstVector.begin(), firstVector.end());
          std::cout << std::endl << "R2" << " ";
          printContainer(secondVector.begin(), secondVector.end());
          std::cout << std::endl << "R3" << " ";
          printContainer(thirdVector.begin(), thirdVector.end());
          std::cout << std::endl << std::endl << "PS" << " ";
          if (lastSign == 0) {
            std::cout << "+" << std::endl;
          } else if (lastSign == 1) {
            std::cout << "-" << std::endl;
          }
          std::cout << "PC" << " " << commandsCounter << std::endl << "TC" << " " << tactsCounter << std::endl;
          std::cout << "..................................." << std::endl;
}

