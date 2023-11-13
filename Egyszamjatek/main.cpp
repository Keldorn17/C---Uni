#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

class Data {
public:
    std::vector<int> fordulo;
    std::string name;

    explicit Data(const std::string& data) {
        std::istringstream iss(data);
        int num;

        while (iss >> num)
            fordulo.push_back(num);

        iss.clear();
        std::getline(iss, name);
    }
};

int main() {

    std::vector<Data> dataList;

    std::ifstream inputFile;
    inputFile.open("..//egyszamjatek.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line))
        dataList.emplace_back(line);

    // 3td task
    std::cout << "\t3. feladat: Jatakosok szama: " << dataList.size() << std::endl;

    // 4th task
    std::cout << "\t4. feladat: Fordulok szama: " << dataList[0].fordulo.size() << std::endl;

    // 5th task
    bool guessedOne = false;

    for (auto const& data : dataList) { // (Data const& i : dataList) is also valid solution
        if (data.fordulo[0] == 1) {
            guessedOne = true;
            break;
        }
    }

    if (guessedOne)
        std::cout << "\t5. feladat: Az elso forduloban volt egyes tipp!" << std::endl;
    else
        std::cout << "\t5. feladat: Az elso forduloban nem volt egyes tipp!" << std::endl;

    // 6th task
    int biggestGuess = 0;

    for (Data const& data : dataList)
        for (int i : data.fordulo) // (int i = 0; i < data.fordulo.size(); i++) also valid
            if (i > biggestGuess)
                biggestGuess = i;

    std::cout << "\t6. feladat: A legnagyobb tipp a fordulok soran: " << biggestGuess << std::endl;

    // 7th task
    int roundNum = 0;

    while (!(roundNum > 0 && roundNum <= dataList[0].fordulo.size())) {
        std::cout << "\t7. feladat: Kerem a fordulo sorszamat [1 - " << dataList[0].fordulo.size() << "]:";
        std::cin >> roundNum;
    }
    roundNum--;

    // 8th task
    int minGuess = 99;

    for (auto const& data : dataList)
        if (data.fordulo[roundNum] < minGuess)
            minGuess = data.fordulo[roundNum];

    std::cout << "\t8. feladat: A nyertes tipp a megadott forduloban: " << minGuess << std::endl;

    // 9th task
    int foundMin = 0;
    std::string nameOfTheWinner;

    for (auto const& data : dataList)
        if (data.fordulo[roundNum] == minGuess) {
            foundMin ++;
            nameOfTheWinner = data.name;
        }

    if (foundMin == 1)
        std::cout << "\t9. feladat: A megadott fordulo nyertese: " << nameOfTheWinner << std::endl;
    else
        std::cout << "\t9. feladat: Nem volt nyertese a fordulonak." << std::endl;

    // 10th task
    if (foundMin == 1) {
        std::ofstream outFile("..//nyertes.txt");

        if (outFile.is_open()) {
            outFile << "Fordulo sorszam: " << roundNum << std::endl;
            outFile << "Nyertes tipp: " << minGuess << std::endl;
            outFile << "Nyertes jatekos: " << nameOfTheWinner << std::endl;

            outFile.close();
            std::cout << "File written successfully.\n";
        }
        else
            std::cerr << "Unable to open the file.\n";
    }


    return 0;
}
