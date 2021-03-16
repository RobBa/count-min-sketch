/**
 * @file countminsketch_test.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Testing the data structure.
 * @version 0.1
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "countminsketch.h"

#include <vector>
#include <algorithm>

#include <iostream>
#include <cassert>
#include <cstdlib>

void test(int nNumbers, int maxVal, int depth, int width){

  std::cout << "nNumbers: " << nNumbers << ", maxVal: " << maxVal << ", depth: " << depth << ", width: " << width << std::endl;

  std::vector<int> randomNumbers;
  for(int i = 0; i < nNumbers; ++i){
    randomNumbers.push_back(rand() % maxVal);
  }

  auto countmin = CountMinSketch<int>(depth, width);

  for(int number: randomNumbers){
    countmin.store(number);
  }
  
  for(int number: randomNumbers){
    auto counted = countmin.getCount(number);
    assert(counted > 0);
    auto real = std::count(randomNumbers.begin(), randomNumbers.end(), number);
    if(counted != real){
      std::cout << "Number: " << number << ", counted: " << counted << ", real: " << real << std::endl;
    }
  }
}

int main(){
  test(5000, 100, 1000, 1000);
  test(5000, 100, 66, 80);
}