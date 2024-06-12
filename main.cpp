#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <NumCpp.hpp>

// Function to read data from a text file into an NdArray
nc::NdArray<int> readTxtToNdArray(const std::string &filePath)
{
  std::ifstream file(filePath);
  std::string line;
  std::vector<std::vector<int>> data;

  while (std::getline(file, line))
  {
    std::stringstream ss(line);
    std::vector<int> row;
    int value;
    while (ss >> value)
    {
      row.push_back(value);
    }
    data.push_back(row);
  }

  // Convert the 2D vector to a NumCpp NdArray
  size_t numRows = data.size();
  size_t numCols = data[0].size();
  nc::NdArray<int> array(numRows, numCols);

  for (size_t i = 0; i < numRows; ++i)
  {
    for (size_t j = 0; j < numCols; ++j)
    {
      array(i, j) = data[i][j];
    }
  }

  return array;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
    return 1;
  }

  std::string inputFilePath = argv[1];
  std::string outputFilePath = argv[2];

  // Read the data from the input text file into an NdArray
  nc::NdArray<int> data = readTxtToNdArray(inputFilePath);

  // Print the data to verify it's been read correctly
  std::cout << "Data from file:" << std::endl;
  std::cout << data << std::endl;

  // Save the NdArray data to the output binary file using nc::dump
  data.dump(outputFilePath);

  return 0;
}
