#include "../include/commandLineParser.h"
#include <string>

bool CommandLineParser::tryParse(int argc, char **argv) {
    _parsedSuccess=false;
    // Check the number of parameters
    if (argc < 2) {        
        LOG_ERROR("Provide an Obj file and a plane description as arguments");
        return false;
    }
    
 
    // check if file is a OBJ file
    _filePathOfObj= std::string(argv[1]);
    if(_filePathOfObj.substr(_filePathOfObj.find_last_of(".") + 1) != "obj" && _filePathOfObj.substr(_filePathOfObj.find_last_of(".") + 1) != "OBJ"  ) {
        LOG_ERROR("The data file '" + _filePathOfObj +  "' is not a OBJ file.");
        return false;
    } 

    // check if file is a JSON file
    _filePathOfPlaneDescription = std::string(argv[2]);
    if(_filePathOfPlaneDescription.substr(_filePathOfPlaneDescription.find_last_of(".") + 1) != "json" && _filePathOfPlaneDescription.substr(_filePathOfPlaneDescription.find_last_of(".") + 1) != "JSON"  ) {
        LOG_ERROR("The plane description file '" + _filePathOfPlaneDescription +  "' is not a JSON file.");
        return false;
    } 
    _parsedSuccess=true;
    return _parsedSuccess;
}
