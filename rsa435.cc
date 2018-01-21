// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 

#include <utility>
#include <random>

#include <chrono>

// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

/// JACOB LIDDY:
  // Prototypes for functions:
BigInteger generate_prime(int bit_size, std::default_random_engine engine);
//This function uses:
  bool fermat(BigInteger base);
  bool miller_rabin(BigInteger tests);
  BigInteger powerOfTwo(int power);

BigInteger lcm(BigInteger, BigInteger);
std::pair<BigInteger, BigInteger> generate_private();
std::pair<BigInteger, BigInteger> generate_public();
 


int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
		      
      //Test Cases:


/*
      std::cout << "a couple of test cases for 3460:435/535 Algorithms!!!\n";
      BigInteger big1 = BigInteger(1);
      for (int i=0;i<400;i++) {
         big1 = big1*10 +rand();
      }
      std::cout << "\nmy big1 !!!\n";
      std::cout << big1;

      BigInteger big2 = BigInteger(1);
      for (int i=0;i<400;i++) {
         big2 = big2*10 +rand();
      }
      std::cout << "\nmy big2 !!!\n";
      std::cout << big2;
      std::cout << "\nmy big3 = big1*big2 !!!\n";
      BigInteger big3 = big1*big2;
      std::cout <<big3;
      std::cout << "\nmy big3/big2 !!!\n";
      std::cout <<big3/big2;
*/      

  unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed1);

  BigInteger b1 = generate_prime(4, gen);
  std::cout << "4 bit Prime:"<< b1 << std::endl;


	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}


BigInteger generate_prime(int bit_size, std::default_random_engine gen){

  BigInteger prime(1);

  do{

    while(!fermat(prime)){

      //First bit MUST BE a 1, ruling all even numbers out.
      //Last bit MUST BE a 1, otherwise prime is too small.
      for(int bit = 1; bit < bit_size; ++bit){

        if (std::uniform_int_distribution<int>(0,1)(gen)) {
          prime += powerOfTwo(bit);
        }

      }

      prime += powerOfTwo(bit_size);

    }
    //After middle numbers are added, finish by adding on large bit.

  } while(!miller_rabin(1));//How many test should we do?


  return prime;
}

BigInteger powerOfTwo(int size){
  BigInteger two_power = BigInteger(1);

  for(int i=1; i <= size; ++i){
    two_power *= BigInteger(2); 
  }

  return two_power;
}


bool fermat(BigInteger p_canidate){
  if (p_canidate == BigInteger(1)){
    return false;
  }
  return true;
}

bool miller_rabin(BigInteger p_canidate){
  return true;
}
