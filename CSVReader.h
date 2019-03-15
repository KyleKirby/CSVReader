/*
 * CSVReader.h
 * Contains a class for reading from CSV files
 *
 * To read a csv file with this class you really only need 2 statements:
 * auto reader = CSVReader(fileName); (where fileName is a string containing the path to the file)
 * auto result = reader.readCSV()
 *
 * The result (as in the one from the code above) is a pointer to a vector of vectors of strings.
 * Or more simply put, it is a 2-d array of strings (contained in vectors).
 *
 * Additional constructors exist to suit the user's needs.
 * Optional parameters to constructors are:
 * - line length, the length in chars of any given line in the CSV file (default is 2000)
 * - delimiter, the char that separates values in the csv file (default is a comma ',')
 *
 *
 * Author: Kyle Kirby
 *
 */

#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <fstream>
#include <vector>

using std::vector;
using std::string;
class CSVReader {
public:
    explicit CSVReader(string &fileName);
    CSVReader(string &fileName, int lineLength);
    CSVReader(string &fileName, char delimiter);
    CSVReader(string &fileName, int lineLength, char delimiter);
    vector<vector<string>>* readCSV(); // return 2-d array of strings representing elements in the csv
    bool openCSV(); // open the CSV file
    bool closeCSV(); // close the CSV file
    vector<string>* readLine(); // read a line from the csv file
    ~CSVReader(); // destructor

private:
    string fileName; // String containing the .csv file to read from.
    char delimiter; // The character to split the csv on. Default is ','
    int lineLength; // This determines the size of the buffer that reads in each line of the csv
    std::ifstream ifs;
    char* buf; // buffer that holds input from .csv file
    char* endBuf; // end of the buffer, used to ensure we don't go past it

};


#endif //CSVREADER_H
