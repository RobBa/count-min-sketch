/**
 * @file hashfunction.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Implementation of hash function with inner state.
 * @version 0.1
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _HASH_FUNCTION_
#define _HASH_FUNCTION_

#include <functional>
#include <cstdlib>

template<typename T>
class HashFunction{
private:
  const int seed;
  static const std::hash<T> objHash;
  static const std::hash<int> intHash;
  
public:
  HashFunction() : HashFunction(rand()) {}
  HashFunction(int seed) : seed(seed) {}

  /**
   * @brief Hashes an object, returns a size_t object. See https://en.cppreference.com/w/cpp/utility/hash
   * 
   * @param obj The object to hash.
   * @return std::size_t The hashed object.
   */
  inline std::size_t hash(const T& obj) const noexcept {
    std::size_t h1 = objHash(obj);
    std::size_t h2 = intHash(this->seed);

    return h1 ^ (h2 << 1);
  }
};

#endif