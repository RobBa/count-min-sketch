/**
 * @file countminsketch.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief C++ implementation of Count-Min-Sketch data structure.
 * @version 0.1
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _COUNT_MIN_SKETCH_
#define _COUNT_MIN_SKETCH_

#include "hashfunction.h"

#include <vector>

#include <iostream>
#include <limits>
#include <cassert>

template<typename T>
class CountMinSketch{
  private:
    const int width, depth;
    std::vector< std::vector<unsigned int> > counts; // counts stored in table
    std::vector< HashFunction<T> > hashFunctions;

  public:
    CountMinSketch() = delete;
    CountMinSketch(int width, int depth);

    void store(const T data) noexcept;
    const unsigned int getCount(const T data);
};

/*--------------------------------- Implementations ------------------------------------*/

/**
 * @brief Construct a new Count Min Sketch< T>:: Count Min Sketch object
 * 
 * @tparam T The type of the class.
 * @param width The width of the sketch.
 * @param depth The depth of the sketch, which corresponds to the number of hash-functions used.
 */
template<typename T>
inline CountMinSketch<T>::CountMinSketch(int width, int depth) : width(width), depth(depth){

  counts.resize(depth);
  for(int i = 0; i < depth; ++i){
    counts[i].resize(width);
  }

  hashFunctions.resize(depth);
}

/**
 * @brief Store a count of data in data structure.
 * 
 * @tparam T Type of data to be stored.
 * @param data The data to store.
 */
template<typename T>
inline void CountMinSketch<T>::store(const T data) noexcept {
  for(int i = 0; i < depth; ++i){
    const auto hashedIndex = hashFunctions[i].hash(data) % width;
    assert(hashedIndex >= 0);
    counts[i][hashedIndex] += 1;
  }
}

/**
 * @brief Retrieve count of data in data structure.
 * 
 * @tparam T Type of data to be stored.
 * @param data The data to store.
 */
template<typename T>
inline const unsigned int CountMinSketch<T>::getCount(const T data){
  unsigned int minVal = std::numeric_limits<unsigned int>::max();
  
  for(int i = 0; i < depth; ++i){
    const auto hashedIndex = hashFunctions[i].hash(data) % width;
    minVal = std::min(minVal, counts[i][hashedIndex]);
  }

  return minVal;
}

#endif