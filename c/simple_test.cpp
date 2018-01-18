#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "words_to_nums.h"
#include "words_to_nums_fr.h"
#include "words_to_nums_es.h"
//#include "bigint.h"


int main(){

  //words_to_nums converter;
  //words_to_nums_fr converter;
  words_to_nums_es converter;
  /*
  std::string test1= "three hundred thousand six hundred twenty";
  std::cout << converter.converter(test1) << std::endl;

  std::cout << converter.reverse(converter.converter(test1)) << std::endl;
  */

  int test2 = 337936947;
  bigint test2bi = bigint(test2);
  std::cout << converter.reverse(test2) << std::endl;

  int test3 = 110438;
  bigint test2bi2 = bigint(test3);
  std::cout << converter.reverse(test3) << std::endl;
  
  srand(time(NULL));
  int testint;
  bigint value;
  for(int j = 0; j < 100; j++){
    testint = rand();
    value = bigint(testint);
    //std::cout << value << std::endl;
    //std::cout << converter.reverse(value) << std::endl;
    if(value != converter.converter(converter.reverse(value)))
      std::cout << "fail with value: " << value << std::endl;

  }
  

  return 0;

}
