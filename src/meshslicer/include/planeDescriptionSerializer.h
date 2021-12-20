#pragma once

#include <fstream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "log.h"

#include "rapidjson/document.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

class PlaneDescriptionSerializer {

public:   
    
    static void deserialize( const char* fileName, glm::vec3 & planeOrg, glm::vec3 & planeNormal ) {
        LOG_DEBUG(fileName);
        std::ifstream f(fileName);
        std::ostringstream ss;
        ss << f.rdbuf(); // reading data
    
        std::string json = ss.str();
        rapidjson::Document doc;
        doc.Parse(json.data());
        if (doc.HasParseError()) {
            LOG_CRITICAL_THROW("invalid JSON found in config");        
        }
        if (doc.IsObject() == false) {
            LOG_CRITICAL_THROW("unexpected JSON format, expected object as root: ");
        }

        if ( ! doc.HasMember("origin")) {
            LOG_ERROR("No origin found to parse");
            return; // no system settings to change
        }
        if ( ! doc.HasMember("normal")) {
            LOG_ERROR("No normal found to parse");
            return; // no system settings to change
        }
        
        rapidjson::Value& originVal = doc["origin"];
        rapidjson::Value& normalVal = doc["normal"];

        int index = 0;
        for (auto& v : originVal.GetArray()) {
            planeOrg[index]=v.GetDouble();
            index++;
        }
        if (index != 3) {
            LOG_CRITICAL_THROW("wrong origin format")
        }

        index = 0;
        for (auto& v : normalVal.GetArray()) {
            planeNormal[index]=v.GetDouble();
            index++;
        }
        if (index != 3) {
            LOG_CRITICAL_THROW("wrong normal format")
        }
        
    }
};