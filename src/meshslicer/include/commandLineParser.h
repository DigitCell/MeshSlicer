#pragma once


#include "log.h"

class CommandLineParser {
    public:
        CommandLineParser(){};
        bool tryParse(int argc, char **argv);


        std::string getObjFilePath() { 
            if (! _parsedSuccess) {
                LOG_CRITICAL_THROW("Parse before reading the OBJ file path")
            }
            return _filePathOfObj;
        }
        std::string getPlaneDescriptionFilePath() { 
            if (! _parsedSuccess) {
                LOG_CRITICAL_THROW("Parse before reading the plane description file path")
            }
            return _filePathOfPlaneDescription;
        }
    
    private:
        bool _parsedSuccess =false;
        std::string _filePathOfObj;
        std::string _filePathOfPlaneDescription;
};