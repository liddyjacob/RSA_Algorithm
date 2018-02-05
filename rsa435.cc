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
BigUnsigned generate_prime(int bit_size, std::default_random_engine& engine);
//This function uses:
  bool fermat(BigUnsigned base);
  bool miller_rabin(BigInteger tests);
  BigInteger powerOfTwo(int power);

BigInteger lcm(BigInteger, BigInteger);
std::pair<BigInteger, BigInteger> generate_private();
std::pair<BigInteger, BigInteger> generate_public();
 
void savebigint(BigUnsigned n, std::string filename);
void savepair(..., std::string filename);

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed1);

  BigUnsigned b1 = generate_prime(4, gen);
  std::cout << "4 bit Prime:"<< b1 << std::endl;
  BigUnsigned b2 = generate_prime(4, gen);
  std::cout << "4 bit Prime:" << b2 << std::endl;
  BigUnsigned b3 = generate_prime(128, gen);
  std::cout << "128 bit Prime:" << b3 << std::endl;

  BigUnsigned b4 = generate_prime(1024,gen);
  std::cout << "1024 bit prime:" << b4; 



	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}




BigUnsigned generate_prime(int bit_size, std::default_random_engine& gen){

  BigUnsigned prime(1);

  //First bit MUST BE a 1, odd numbers only.
  //Last bit MUST BE a 1, otherwise prime is too small.
  for(int bit = 1; bit < bit_size; ++bit){

    if (std::uniform_int_distribution<int>(0,1)(gen)) {
      prime.setBit(bit, 1);
    }

  }

  prime.setBit(bit_size,1);//Makes sure prime is large.

  while(!fermat(prime)){
    prime+=BigUnsigned(2);// Keep adding until prime.
  }


  return prime;
}

BigInteger powerOfTwo(int size){

  BigInteger two_power = BigInteger(1);

  for(int i=1; i <= size; ++i){
    two_power *= BigInteger(2); 
  }

  return two_power;
}
 

bool fermat(BigUnsigned p_canidate){
  if (p_canidate == BigUnsigned(1)){
    return false;
  }

  BigUnsigned exp = p_canidate - BigUnsigned(1);
  BigUnsigned mod = p_canidate;

  //Two tests: a = 2,3
  if (BigInteger(1) != modexp(BigInteger(2), exp, mod))
    return false;
  if (BigInteger(1) != modexp(BigInteger(3), exp, mod))
    return false;

  return true;
}

//Some super-garbage-hack ways of getting unsigneds.
//  std::string expstr(bigIntegerToString(p_canidate - BigInteger(1)));
//  std::string modstr(bigIntegerToString(p_canidate));  



