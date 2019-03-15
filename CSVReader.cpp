/*
 * CSVReader.cpp
 * Contains implementation of methods defined in CSVReader.h
 *
 * Author: Kyle Kirby
 *
 *
 */

#include "CSVReader.h"
#include <iostream>
#include <cstring>

#define defaultLineLength 2000 // set the default line length (affects how much memory is initially allocated to buf)
#define escapeChar '"' // char used to escape special chars in the context of csv (the delimiter, \r, \n)


// minimal constructor, only file name given
CSVReader::CSVReader(string &fileName):
fileName(fileName),
delimiter(','),
lineLength(defaultLineLength)
{
    this->buf = new char[this->lineLength+1];
    this->endBuf = buf + this->lineLength;
}


// constructor with file name and line length
CSVReader::CSVReader(string &fileName, int lineLength):
fileName(fileName),
delimiter(','),
lineLength(lineLength)
{
    this->buf = new char[this->lineLength+1];
    this->endBuf = buf + this->lineLength;
}

// constructor with file name and delimiting char
CSVReader::CSVReader(string &fileName, char delimiter):
        fileName(fileName),
        delimiter(delimiter),
        lineLength(defaultLineLength)
{
    this->buf = new char[this->lineLength+1];
    this->endBuf = buf + this->lineLength;
}

// constructor with file name, line length, and delimiting char
CSVReader::CSVReader(string &fileName, int lineLength, char delimiter):
        fileName(fileName),
        delimiter(delimiter),
        lineLength(lineLength)
{
    this->buf = new char[this->lineLength+1];
    this->endBuf = buf + this->lineLength;
}


// open csv file at the location of this->fileName (given in constructor)
bool CSVReader::openCSV(){
    this->ifs.open(this->fileName);
    return this->ifs.is_open();
}

// close the csv file
bool CSVReader::closeCSV(){
    this->ifs.close();
    return !this->ifs.is_open();
}


// read one line from the csv file
vector<string>* CSVReader::readLine() { // read a line from the csv file
    auto *lineText = new vector<string>();
    char* word = this->buf; // points at beginning of buf
    char* ptr = this->buf; // ptr used to step through the buffer and parse it
    bool escaping = false; // set to true when escaping characters (inside double quotes)

    *this->endBuf = '\0'; // insert null char at end of buf
    int nextChar;
    while (__predict_true(this->ifs.good())) { // make there is more input to read from
        nextChar = this->ifs.get();
        if(__predict_false(nextChar == EOF)){
            *ptr = '\0';
            lineText->emplace_back(word);
            return lineText;
        }
        *ptr = nextChar;
        if(nextChar == this->delimiter && __predict_true(!escaping)){ // when encountering the delimiter, most of the time it won't be escaped (ideally)
            *ptr = '\0';
            lineText->emplace_back(word);
            ptr++;
            word = ptr;
            continue;
        }
        switch(nextChar){
            case '\r':
                if(!escaping && this->ifs.good() && this->ifs.peek() == '\n') // if we aren't escaping, check if next char is \n, and if it is then get it so we can skip over it
                    ifs.get(); // do nothing with it (we want to skip it)
            case '\n':
                if(!escaping){ // if we aren't escaping we've reached the end of the line
                    *ptr = '\0';
                    lineText->emplace_back(word);
                    return lineText; // nothing more needs to be done
                }
                break; // reaching here means we are escaping (with ") and we treat it as a normal char
            case escapeChar:
                escaping = !escaping; // flip the value of escaping
                break;
            case '\0': // leaving this here just in case
                lineText->emplace_back(word);
                return lineText;
            default:
                ;
        }
        ptr++;
        if(__predict_false(ptr == endBuf)){ // buf is too small and we need to reallocate it
            this->lineLength *= 2;
            auto buf2 = new char[this->lineLength + 1];
            strncpy(buf2, this->buf, this->endBuf - this->buf);
            delete this->buf;
            this->buf = buf2;
            this->endBuf = this->buf + this->lineLength;
            *this->endBuf = '\0';
        }
    }

    return lineText;

}

// read all lines from the csv file
vector<vector<string>>* CSVReader::readCSV(){
    if(!this->openCSV()){ // if the file was unable to be opened, print an error and return nullptr
        std::cerr << "Error: could not open " << this->fileName << "\n";
        return nullptr;
    }
    auto *fileText = new vector<vector<string>>();
    while (__predict_true(this->ifs.good())) { // while more input is available, get the next line
        fileText->emplace_back(*this->readLine());
    }
    this->closeCSV();
    return fileText;
}

// destructor
CSVReader::~CSVReader(){
    delete this->buf;
}
