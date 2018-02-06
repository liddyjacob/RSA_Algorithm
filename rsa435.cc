// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 

#include <utility>
#include <random>

#include <chrono>
#include <fstream>

#include <thread>

// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

/// JACOB LIDDY:
  // Prototypes for functions:
std::pair<BigUnsigned, BigUnsigned>
generate_primes(int bit_size, std::default_random_engine& engine);
//This function uses:
  void find_prime(BigUnsigned& prime);
  bool fermat(BigUnsigned base);


BigInteger lcm(BigInteger, BigInteger);
std::pair<BigInteger, BigInteger> generate_private();
std::pair<BigInteger, BigInteger> generate_public();
 
void saveBigUnsigned(BigUnsigned n, std::string filename);
//void savepair(..., std::string filename);

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed1);

  std::pair<BigUnsigned, BigUnsigned> p1 = generate_primes(1024, gen);
  std::cout << "1024 bit Prime:"<< p1.first << std::endl;
  std::cout << "1024 bit Prime:" << p1.second << std::endl;
  
  saveBigUnsigned(p1.first, "p_q.txt");
  saveBigUnsigned(p1.second, "p_q.txt");

	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}

std::pair<BigUnsigned, BigUnsigned> generate_primes(int bit_size, std::default_random_engine& gen){

  BigUnsigned prime1(1);
  BigUnsigned prime2(1);
  //First bit MUST BE a 1, odd numbers only.
  //Last bit MUST BE a 1, otherwise prime is too small.
  for(int bit = 1; bit < bit_size; ++bit){

    if (std::uniform_int_distribution<int>(0,1)(gen)) {
      prime1.setBit(bit, 1);
    }
    if (std::uniform_int_distribution<int>(0,1)(gen)){
      prime2.setBit(bit,1);
    }
    
  }

  prime1.setBit(bit_size,1);//Makes sure prime is large.
  prime2.setBit(bit_size,1);

  std::thread t1(find_prime, std::ref(prime1));
  std::thread t2(find_prime, std::ref(prime2));

  t1.join();
  t2.join();

  return std::pair<BigUnsigned, BigUnsigned>(prime1, prime2);

}


void find_prime(BigUnsigned& prime){

  while(!fermat(prime)){
    prime+=BigUnsigned(2);// Keep adding until prime.
  }

}

void fermat_exp(BigInteger base, BigUnsigned power, 
            BigUnsigned mod, bool& pass)
{

  if (BigInteger(1) != modexp(base, power, mod))
    pass = false;
  else
    pass = true;
 
}

bool fermat(BigUnsigned p_canidate){
  if (p_canidate == BigUnsigned(1)){
    return false;
  }

  BigUnsigned exp = p_canidate - BigUnsigned(1);
  BigUnsigned mod = p_canidate;

  bool pass1 = false;
  bool pass2 = false;

  std::thread t1(fermat_exp, BigInteger(2), exp, mod, std::ref(pass1));
  std::thread t2(fermat_exp, BigInteger(3), exp, mod, std::ref(pass2));

  t1.join();
  t2.join();

  return (pass1 && pass2);
}




void saveBigUnsigned(BigUnsigned p, std::string filename){
  // Open the file:
  std::ofstream savefile;
  savefile.open(filename.c_str(), std::ios::app);

  if (savefile.is_open()){

    savefile << p;
    savefile << std::endl;

  } 
  else {
    std::cerr << "p not saved to file! Failed to open\n";
  }
  // Close the ifle 
  savefile.close();
}




//Some super-garbage-hack ways of getting unsigneds.
//  std::string expstr(bigIntegerToString(p_canidate - BigInteger(1)));
//  std::string modstr(bigIntegerToString(p_canidate));  



