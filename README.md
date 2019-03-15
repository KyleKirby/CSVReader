# CSVReader
### A class for reading in .csv files using C++


To read a csv file with this class you really only need 2 statements:   
auto reader = CSVReader(fileName);  // where fileName is a string containing the path to the file    
auto result = reader.readCSV();

The result (as in the one from the code above) is a pointer to a vector of vectors of strings.
Or more simply put, it is a 2-d array of strings (contained in vectors).

Additional constructors exist to suit the user's needs.
Optional parameters to constructors are:
- line length, the length in chars of any given line in the CSV file (default is 2000)
- delimiter, the char that separates values in the csv file (default is a comma ',')
