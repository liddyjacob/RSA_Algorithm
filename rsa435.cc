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

/// JACOB LIDDY


// Prototypes for functions:


/* generate_primes: Generates two primes, returns an std::pair
 * of primes as BigUnsigned 
 * 
 * Parameters:
 *   bit_size: the size of the prime in terms of binary digits.
 *   engine: the random-generating engine for our prime */


std::pair<BigUnsigned, BigUnsigned>
generate_primes(int bit_size, std::default_random_engine& engine);
//This function uses:
  void find_prime(BigUnsigned& prime); // For threads
  bool fermat(BigUnsigned base);

// Generate exponents e, d based off p and q.
std::pair<BigInteger, BigInteger> 
generate_exps(BigUnsigned p, BigUnsigned q);

// Append to the end of a file 
template <typename T>
void saveNumber(T n, std::string filename);

//Save the keys in desired files:
void saveKey(std::pair<BigUnsigned, BigUnsigned> p_q,
             std::pair<BigInteger, BigInteger>   e_d,
             std::string f1name, std::string f2name); 
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

  saveNumber(p1.first, "p_q.txt");
  saveNumber(p1.second, "p_q.txt");

  std::pair<BigInteger, BigInteger> e_d 
    = generate_exps(p1.first, p1.second);

  saveKey(p1, e_d, "e_n.txt", "d_n.txt");


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



template <typename T>
void saveNumber(T n, std::string filename){
  // Open the file:
  std::ofstream savefile;
  savefile.open(filename.c_str(), std::ios::app);

  if (savefile.is_open()){
    savefile << n;
    savefile << std::endl;
  } 
  else {
    std::cerr << "n not saved to file! Failed to open\n";
  }
  // Close the ifle 
  savefile.close();
}
void saveKey(std::pair<BigUnsigned, BigUnsigned> p_q,
             std::pair<BigInteger, BigInteger>   e_d,
             std::string f1name, std::string f2name){

  BigUnsigned n = p_q.first * p_q.second;
  
  saveNumber(n, f1name);
  saveNumber(e_d.first, f1name);

  saveNumber(n, f2name);
  saveNumber(e_d.second, f2name);

}
// Returns e,d:
std::pair<BigInteger, BigInteger> 
generate_exps(BigUnsigned p, BigUnsigned q){

  auto pos = BigInteger::positive;

  BigInteger p_(p, pos);
  BigInteger q_(q, pos);

  BigInteger one(1);

  BigInteger gcd(0);
 
  BigInteger e(39); // Start at 41, likely that it is coprime.
  BigInteger d(0); // Better not be zero when we are done.

  BigInteger euler_phi = (p_ - one)*(q_ - one);
  BigInteger dummy(0);

  while (gcd != one){
    
    e = e + one + one;
    extendedEuclidean(euler_phi, e, gcd, dummy, d);
  }

  return std::pair<BigInteger, BigInteger>(e,d % euler_phi);
}



