#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <iomanip>

class Data {
public:
    std::string fullName;
    int startNumber{}, firstResult, secondResult, weight{}, sumOfResults;

    explicit Data(const std::vector<std::string>& data) {
        firstResult = std::stoi(data.at(0));
        secondResult = std::stoi(data.at(1));
        fullName = data.at(2) + " " + data.at(3);
        sumOfResults = firstResult + secondResult;
        Weight();
        StartNumber();
    }
private:

    void Weight () {
        weight = 0;
        while (!(weight >= 95 && weight <= 150)) {
            std::cout << "Give me " << fullName << "'s weight (95-150kg): ";
            std::cin >> weight;
        }
    }

    void StartNumber() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(10, 99);
        startNumber = dist(gen);
    }
};

int main() {
    std::vector<Data> dataList;

    std::ifstream inputFile;
    inputFile.open("..//nevek.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token)
            tokens.push_back(token);
        dataList.emplace_back(tokens);
    }

    // 4th task
    for (auto const& data : dataList)
        std::cout << std::left << std::setw(5) << data.startNumber << std::setw(20) << data.fullName
                  << std::setw(5) << data.firstResult << std::setw(5) << data.secondResult << std::setw(5)
                  << data.sumOfResults << std::endl;

    // 5th - 6th task
    int sum = 0;
    for (auto const& data : dataList) {
        if (data.sumOfResults >= 400)
            std::cout << data.fullName << " pulled " << data.sumOfResults << "kg weight in combined." << std::endl;
        if (data.weight < 100)
            sum++;
    }

    std::cout << sum << " people weighted less then 100 kg." << std::endl;

    std::cout << dataList.at(dataList.size() - 1).fullName << " was the last person to attend the "
                                                              "competition with a start number of "
                                                              << dataList.at(dataList.size() - 1).startNumber
                                                              << std::endl;

    std::ofstream outFile("..//results.txt");

    if (outFile.is_open()) {
        for (auto const& data : dataList)
            outFile << data.startNumber << " " << data.fullName << " " << data.firstResult << " " << data.secondResult
                    << " " << data.sumOfResults << " " << data.weight << std::endl;
        outFile.close();
        std::cout << "File written successfully.\n";
    }else
        std::cerr << "Unable to open the file.\n";

    return 0;
}
