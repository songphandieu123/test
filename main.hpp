#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>

namespace Hidden{
  void ParseLine(std::string& line){
    int lineStart = 0;
    for(unsigned int i = 0; i < line.length(); i++){
      if(line[i] != ' '){
        lineStart = i;
        break;
      }
    }
    for(unsigned int i = lineStart; i < line.length(); i++){
      if(line[i] == '\"' || line[i] == ':' || line[i] == ',' ){
        line[i] = ' ';
      }
    }
  }

  std::string TypeCheck(std::string line){
    if(line.length() == 0){
      std::cerr << "Could not determine type\n";
      return "ur gae\n";
    }
    else{
      if(line[line.length() - 1] == '\"'){
        return "string";
      }
      else if(line[line.length() - 1] == 'e'){
        return "bool";
      }
      else if(line.find(",") != std::string::npos){
        return "float";
      }
      else{
        return "int";
      }
    }
  }
    
  std::stringstream FindValue(std::ifstream& file, std::string variableName){
    if(!file.is_open()){
      std::cerr << "File not open\n";
    }
    else{
      std::string line, key;
      while(std::getline(file, line)){
        if(line != "{" && line != "}"){
          Hidden::ParseLine(line);
        std::cout << line << '\n';

          std::stringstream value(line);
          value >> key;
          if(key == variableName){
            return value;
          }
        }
      }
      std::cerr << "Could not find key " << variableName << " in file\n";
    }
    exit(1);
  }

  std::string GetOldValue(std::string& fileString, std::string variableName, int variablePosition){
    std::string oldValue;
    for(int i = variablePosition + variableName.length() + 4; fileString[i] != ' ' && fileString[i] != '\n' &&  fileString[i] != ','; i++){
      oldValue += fileString[i];
    }
    return oldValue;
  }

  void EditValue(std::string& fileString, std::string variableName, std::string value, int variablePosition, std::string oldValue){
    fileString.replace(variablePosition + variableName.length() + 4, oldValue.length(), value);
  }
};


int FindIntValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = Hidden::FindValue(file, variableName);
  if(Hidden::TypeCheck(outputStream.str()) == "int"){
    int output;
    outputStream >> output;
    return output;
  }
  else{
    std::cerr << variableName << " is of " << Hidden::TypeCheck(outputStream.str()) <<  " type!\n";
    exit(1);
  }
}

float FindFloatValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = Hidden::FindValue(file, variableName);
  if(Hidden::TypeCheck(outputStream.str()) == "float"){
    float output;
    outputStream >> output;
    return output;
  }
  else{
    std::cerr << variableName << " is of " << Hidden::TypeCheck(outputStream.str()) <<  " type!\n";
  }
  exit(1);
}

std::string FindStringValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = Hidden::FindValue(file, variableName);
  if(Hidden::TypeCheck(outputStream.str()) == "float"){
    return outputStream.str();
  }
  else{
    std::cerr << variableName << " is of " << Hidden::TypeCheck(outputStream.str()) <<  " type!\n";
  }
  exit(1);
}

bool FindBoolValue(std::ifstream& file, std::string variableName){
  std::stringstream outputStream = Hidden::FindValue(file, variableName);
  std::string output;
  outputStream >> output;
  if(Hidden::TypeCheck(outputStream.str()) == "bool"){
    if(output == "true"){
      return true;
    }
    return false;
  }
  else{
    std::cerr << variableName << " is of " << Hidden::TypeCheck(outputStream.str()) <<  " type!\n";
  }
  exit(1);
}

std::string ReadFileContent(std::ifstream& file){
  if(file.is_open()){
    std::string line;
    std::string output;
    while(std::getline(file, line)){
      output += line;
      output += '\n';
    }
    file.clear();
    file.seekg(0);
    return output;
  }
  else{
    std::cerr << "File is not open!";
  }
  exit(1);
}

void EditIntValue(std::string& fileString, std::string variableName, std::string value){
  int variablePosition = fileString.find(variableName);
  std::string oldValue = Hidden::GetOldValue(fileString, variableName, variablePosition);
  if(Hidden::TypeCheck(oldValue) == "int"){
    Hidden::EditValue(fileString, variableName, value, variablePosition, oldValue);
  }
  else{
    std::cerr << variableName << " is of type " << Hidden::TypeCheck(oldValue) << "!";
  }
}

void EditFloatValue(std::string& fileString, std::string variableName, std::string value){
  int variablePosition = fileString.find(variableName);
  std::string oldValue = Hidden::GetOldValue(fileString, variableName, variablePosition);
  if(Hidden::TypeCheck(oldValue) == "float"){
    Hidden::EditValue(fileString, variableName, value, variablePosition, oldValue);
  }
  else{
    std::cerr << variableName << " is of type " << Hidden::TypeCheck(oldValue) << "!";
  }
}

void EditBoolValue(std::string& fileString, std::string variableName, std::string value){
  int variablePosition = fileString.find(variableName);
  std::string oldValue = Hidden::GetOldValue(fileString, variableName, variablePosition);
  if(Hidden::TypeCheck(oldValue) == "bool"){
    Hidden::EditValue(fileString, variableName, value, variablePosition, oldValue);
  }
  else{
    std::cerr << variableName << " is of type " << Hidden::TypeCheck(oldValue) << "!";
  }
}

void EditStringValue(std::string& fileString, std::string variableName, std::string value){
  int variablePosition = fileString.find(variableName);
  std::string oldValue = Hidden::GetOldValue(fileString, variableName, variablePosition);
  if(Hidden::TypeCheck(oldValue) == "string"){
    Hidden::EditValue(fileString, variableName, "\"" + value + "\"", variablePosition, oldValue);
  }
  else{
    std::cout << oldValue << '\n';
    std::cerr << variableName << " is of type " << Hidden::TypeCheck(oldValue) << "!";
  }
}

void ReplaceFile(std::ofstream& writeFile, std::ifstream& readFile, std::string fileString){
  for(char& c : fileString){
    writeFile << c;
  }
  writeFile.close();
  readFile.close();
  remove("read.json");
  rename("write.json", "read.json");
}