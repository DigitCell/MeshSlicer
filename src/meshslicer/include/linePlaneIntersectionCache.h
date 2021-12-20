#pragma once

#include <map>
#include <exception>
#include <glm/glm.hpp>


// this container holds a cache of calculated intersections 
// the cache is structured to search first for the index pointing to the vertice with biggest z-value 
// and than checking if the second point (=index pointing to vertice with lowest z-value) of the line is present
class LinePlaneIntersectionCache {
    
    private:
        std::map<int,std::map<int,int>> _cache;

    public:
        LinePlaneIntersectionCache(){};
        bool tryRetrieveFromCache (int indexTop, int indexBottom,  int & cacheIndexValue) {
            if (indexTop == indexBottom) {                
                LOG_CRITICAL_THROW("indexTop and indexBottom CANNOT be equal for retrieving from cache");
            }

            if ( _cache.find (indexTop) == _cache.end ()) {
                return false;
            }
            if ( _cache[indexTop].find(indexBottom) ==  _cache[indexTop].end ()) {
                return false;
            }
            cacheIndexValue = _cache[indexTop][indexBottom];
            return true;
        }
        void addToCache (int indexTop, int indexBottom, int cacheIndexValue) {
            if (indexTop == indexBottom) {
                LOG_CRITICAL_THROW("indexTop and indexBottom CANNOT be equal for adding to cache");                
            }
            if ( _cache.find (indexTop) == _cache.end ()) {
                std::map<int,int> newIndexTop;
                newIndexTop[indexBottom] = cacheIndexValue;
                _cache[indexTop] = newIndexTop;
            } else {
                 _cache[indexTop][indexBottom] = cacheIndexValue;
            }
        }
};