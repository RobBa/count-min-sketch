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

    void operator+(const CountMinSketch<T>& other);
    void operator-(const CountMinSketch<T>& other);

    static float cosineSimilarity(const CountMinSketch<T>& c1, const CountMinSketch<T>&c2);

    void store(const T data) noexcept;
    const unsigned int getCount(const T data);

    inline void reset() noexcept {
      for(auto& row: counts){
        for(int i = 0; i < width; ++i){
          row[i] = 0;
        }
      }
    }
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

/**
 * @brief Adds the two count-min-sketches. Can only be done when sizes equal.
 * 
 * @tparam T Type. 
 * @param other The other cms.
 * @return CountMinSketch<T> The result.
 */
template<typename T>
inline void CountMinSketch<T>::operator+(const CountMinSketch<T>& other){
  assert(this->width == other.width && this->depth == other.depth);

	for(int i = 0; i < depth; ++i){
    for(int j = 0; j < width; ++j){
      this->counts[i][j] += other.counts[i][j];
    }
  }
}
 
 /**
 * @brief Subtracts the two count-min-sketches. Can only be done when sizes equal.
 * 
 * @tparam T Type. 
 * @param other The other cms.
 * @return CountMinSketch<T> The result.
 */
template<typename T>
inline void CountMinSketch<T>::operator-(const CountMinSketch<T>& other){
  assert(this->width == other.width && this->depth == other.depth);

	for(int i = 0; i < depth; ++i){
    for(int j = 0; j < width; ++j){
      this->counts[i][j] -= other.counts[i][j];
    }
  }
}

/**
 * @brief Computes average cosine similarity among two sketches.
 * 
 * @param c1 Sketch 1.
 * @param c2 Sketch 2.
 * @return float The average similarity.
 */
template <typename T>
float CountMinSketch<T>::cosineSimilarity(const CountMinSketch<T>& c1, const CountMinSketch<T>&c2){
  assert(c1.width == c2.width && c1.depth == c2.depth);
  float res = 0;

  for(int i = 0; i < c1.depth; ++i){
    const auto& v1 = c1.counts[i];
    const auto& v2 = c2.counts[i];

    float dot = 0, absolute1 = 0, absolute2 = 0;
    for(int j = 0; j < c2.width; ++j){
      dot += v1[j] * v2[j];
      absolute1 += pow(v1[j], 2);
      absolute2 += pow(v2[j], 2);
    }
    absolute1 = sqrt(absolute1);
    absolute2 = sqrt(absolute2);

    res += dot / (absolute1 * absolute2 + 1e-6);
  }

  return res / c1.depth;
}

#endif