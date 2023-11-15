#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <random>
#include <iomanip>

class Data {
public:
    std::string firstName;
    std::string secondName;
    std::string fullName;
    int bestTime{};
    int startNumber{};
    int currentTime{};
    explicit Data(const std::string& dataFile) {
        std::istringstream iss(dataFile);

        iss >> firstName >> secondName >> bestTime;
        fullName = firstName + " " + secondName;
    }
};

int main() {

    std::vector<Data> dataList;

    std::ifstream inputFile;
    inputFile.open("..//indulok.txt");

    if (!inputFile.is_open()) {
        std::cerr << "File opening failure.";
        return 1;
    }
    std::string line;
    while (std::getline(inputFile, line))
        dataList.emplace_back(line);

    // 2nd task
    std::cout << "2nd task: " << dataList.size() << " competitors attended the competition." << std::endl;

    // 3rd task
    std::cout << "3rd task:" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(100, 199);

    for (auto& data : dataList) {
        data.startNumber = dist(gen);
        std::cout << "\tWhat was the current time for " << data.fullName << ":";
        std::cin >> data.currentTime;
    }

    // 4th task
    std::cout << "4th task:" << std::endl;
    std::cout << std::setw(20) << "Full Name" << std::setw(10) << "Best Time" << std::setw(15)
              << "Start Number" << std::setw(15) << "Current Time";
    for (auto const& data : dataList) {
        std::cout << std::setw(20) << data.fullName << std::setw(10) << data.bestTime << std::setw(15)
                  << data.startNumber << std::setw(15) << data.currentTime;
    }

    // 5th task
    int sumTime = 0;

    for (auto const& data: dataList)
        sumTime += data.currentTime;

    std::cout << "\n5th task: The sum of the runners time is " << sumTime << "s" << std::endl;

    // 6th task
    int hasBetterRunNum = 0;

    for (auto const& data : dataList)
        if (data.bestTime > data.currentTime)
            hasBetterRunNum ++;

    std::cout << "6th task: " << hasBetterRunNum << " people had better time then they previous best time. They were:\n";
    for (auto const& data : dataList)
        if (data.bestTime > data.currentTime) {
            std::cout << "\t" << data.fullName << " with a time of " << data.currentTime << "s" << std::endl;
        }

    // 7th task
    double avg = 0.0;

    for (auto const& data : dataList)
        if (data.bestTime < data.currentTime)
            avg += data.currentTime;
    size_t validDataCount = dataList.size() - hasBetterRunNum;

    if (validDataCount > 0)
        avg = avg / static_cast<double>(validDataCount); // (double) validDataCount

    std::cout << "7th task: The average of time for those who did not broke their records is: " << avg << "s" << std::endl;

    // 8th task
    std::cout << "8th task: The first person to apply for the competition was " << dataList[0].fullName
              << " with a starting number of " << dataList[0].startNumber << ". His/Her current time is "
              << dataList[0].currentTime << "s which is " << (dataList[0].currentTime - dataList[0].bestTime)
              << "s compered to his/her best time.";

    // 9th task
    std::ofstream outFile("..//eredmeny.txt");

    if (outFile.is_open()) {
        for (auto const& data : dataList)
            outFile << data.fullName << " " << data.startNumber << " " << data.bestTime << " " << data.currentTime << std::endl;
        outFile.close();
        std::cout << "File written successfully.\n";
    }
    else
        std::cerr << "Unable to open the file.\n";

    return 0;
}
