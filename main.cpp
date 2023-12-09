#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <chrono>

void ParseLine(std::string& line){
  line.erase(0, 4);
  for(unsigned int i = 0; i < line.length(); i++){
    if(line[i] == '\"' || line[i] == ':' || line[i] == ',' ){
      line[i] = ' ';
    }
  }
}

int CountValue(std::string line){
  int valueCount = 1;
  for(unsigned int i = 0; i < line.length(); i++){
    if(line[i] == ','){
      valueCount++;
    }
  }
  return valueCount;
}

std::string TypeCheck(std::string line){
  if(line[line.length() - 1] == ' '){
    return "string";
  }
  else if(line[line.length() - 1] == 'e'){
    return "bool";
  }
  else{
    for(int i = line.length() - 1; i > -1; i--){
      if(line[i] == '.'){
        return "float";
      }
      if(line[i] == ' '){
        return "int";
      }
    }
  }
  std::cerr << "Could not determine type\n";
  exit(1);
  return "ur gae\n";
}

std::stringstream FindValue(std::ifstream& file, std::string variableName, bool isVector = false){
  if(!file.is_open()){
    std::cerr << "File not open\n";
  }
  else{
    std::string line;
    std::string key;
    if(!isVector){
      while(std::getline(file, line)){
        if(line != "{" && line != "}"){
          ParseLine(line);
          std::stringstream value(line);
          value >> key;
          if(key == variableName){
            return value;
          }
        }
      }
    }
    else{
      while(std::getline(file, line)){
        if(line.at(line.length() - 1) == '['){
          ParseLine(line);
          std::stringstream value(line);
          value >> key;
          if(key == variableName){
            std::getline(file, line);
            value.str(std::string()); // clear stream
            value << line.erase(0, 8);
            return value;
          }
        }
    } 
    }
    std::cerr << "Could not find key " << variableName << " in file\n\n";
  }
  exit(1);
}

int FindIntValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = FindValue(file, variableName);
  if(TypeCheck(outputStream.str()) == "int"){
    int output;
    outputStream >> output;
    return output;
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n";
    exit(1);
  }
}

float FindFloatValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = FindValue(file, variableName);
  if(TypeCheck(outputStream.str()) == "float"){
    float output;
    outputStream >> output;
    return output;
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n";
    exit(1);
  }
}

std::string FindStringValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = FindValue(file, variableName);
  if(TypeCheck(outputStream.str()) == "float"){
    return outputStream.str();
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n";
    exit(1);
  }
}

bool FindBoolValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = FindValue(file, variableName);
  std::string output;
  outputStream >> output;
  if(TypeCheck(outputStream.str()) == "bool"){
    if(output == "true"){
      return true;
    }
    return false;
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n";
    exit(1);
  }
}

void FindVectorIntValue(std::ifstream& file, std::string variableName, std::vector<int>& vector){
  std::stringstream lineStream = FindValue(file, variableName, true);
  std::string output = lineStream.str();
  int valueCount = CountValue(output);
  ParseLine(output);
  std::stringstream outputStream(output);
  if(TypeCheck(outputStream.str()) == "int"){
    for(int i = 0; i < valueCount; i++){
      int temp;
      outputStream >> temp;
      vector.push_back(temp);
    }
    vector.shrink_to_fit();
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n";
    exit(1);
  }
}

void FindVectorFloatValue(std::ifstream& file, std::string variableName, std::vector<float>& vector){
  std::stringstream lineStream = FindValue(file, variableName, true);
  std::string output = lineStream.str();
  int valueCount = CountValue(output);
  ParseLine(output);
  std::stringstream outputStream(output);
  if(TypeCheck(outputStream.str()) == "float"){
    for(int i = 0; i < valueCount; i++){
      float temp;
      outputStream >> temp;
      vector.push_back(temp);
    }
    vector.shrink_to_fit();
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n";
    exit(1);
  }
}

void FindVectorStringValue(std::ifstream& file, std::string variableName, std::vector<std::string>& vector){
  std::stringstream lineStream = FindValue(file, variableName, true);
  std::string output = lineStream.str();
  int valueCount = CountValue(output);
  ParseLine(output);
  std::stringstream outputStream(output);
  if(TypeCheck(outputStream.str()) == "string"){
    for(int i = 0; i < valueCount; i++){
      std::string temp;
      outputStream >> temp;
      vector.push_back(temp);
    }
    vector.shrink_to_fit();
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n";
  }
}

void FindVectorBoolValue(std::ifstream& file, std::string variableName, std::vector<bool>& vector){
  std::stringstream lineStream = FindValue(file, variableName, true);
  std::string output = lineStream.str();
  int valueCount = CountValue(output);
  ParseLine(output);
  std::stringstream outputStream(output);
  if(TypeCheck(outputStream.str()) == "bool"){
  for(int i = 0; i < valueCount; i++){
    std::string temp;
    outputStream >> temp;
    if(temp == "true"){
      vector.push_back(true);
    }
    else{
      vector.push_back(false); 
    }
  }
  vector.shrink_to_fit();
  }
  else{
    std::cerr << variableName << " is of " << TypeCheck(outputStream.str()) <<  " type!\n"; 
  }
}

std::string ReadFileContent(std::ifstream& file){
  if(file.is_open()){
    std::string line;
    std::string output;
    while(std::getline(file, line)){
      output += line;
      output += '\n';
    }
    return output;
  }
  else{
    std::cerr << "File is not open!";
    exit(1);
  }
}

void EditIntValue(std::string& fileString, std::string variableName, std::string value){
  int variablePosition = fileString.find(variableName);
  for(unsigned int i = variablePosition + 10; i < variablePosition + 10 + value.length() ; i++){
    fileString[i] = value[i - (variablePosition + 10)];
    std::cout << fileString[i] << '\n';
  }
  int lineEndPosition = variablePosition + 10 + value.length();
  while(fileString[lineEndPosition] != ','){
    std::cout << fileString[lineEndPosition] << '\n'; 
    lineEndPosition++;
  }
  std::cout << fileString[lineEndPosition];
  fileString.erase(variablePosition + 10 + value.length(), lineEndPosition - 2); 
}

void ReplaceFile(std::ofstream& file, std::string fileString){
  for(char& c : fileString){
    file << c;
  }
}

int main(){
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::ifstream read("test.json");
  std::ofstream write("edit.json");
  std::string dataLine;
  if(read.is_open()){
    std::string readString = ReadFileContent(read);
    EditIntValue(readString, "health", "37");
    read.close();
    ReplaceFile(write, readString);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()<< " ms\n";
  }
  else{
    std::cerr << "Failed to open file\n";
  }

  return 0;
}