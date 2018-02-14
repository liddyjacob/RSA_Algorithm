#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <vector>

#include <cstring>

#include "sha256.h"
#include "BigIntegerLibrary.hh"

#define SIGLENGTH 1024

void sign(std::string msgname, std::string privfile);
bool verify(std::string msgname, std::string pubfile);

BigInteger to_dec(std::string hex);

enum Mode {SIGN, VERIFY, VOID};

int main(int argc, char **argv){
  
  if (argc != 3){
    std::cerr << "Usage: ./sign [s,v] [FILE]\n";
    return -1;
  }

  std::vector<std::string> args(argv, argv + argc);
  Mode mode = VOID;

  if ((args[1] == "s") || (args[1] == "S")){
    mode = SIGN;
  } else if ((args[1] == "v") || (args[1] == "V")) {
    mode = VERIFY;
  }

  if (mode == VOID){
    std::cerr << "Invalid mode '" << args[1] << "', choose s or v\n";
    return -1;
  }

  std::ifstream msgfile;
  msgfile.open(args[2].c_str(), std::ios::in | std::ios::binary);

  if (!msgfile.is_open()){
    std::cerr << "Error opening '" << args[2] << "'\n";
    return -1;
  };

  //Close file - was just testing to see if it existed.
  msgfile.close();

  if (mode == SIGN){
    sign(args[2], "d_n.txt");
  }

  if (mode == VERIFY){
    if (verify(args[2], "e_n.txt")){
      std::cout << "File is signed.\n";
    } else {
      std::cout << "File is not signed properly.\n";
    }
  }

  return 0;
}

void sign(std::string msgname, std::string privkey){

  //Need to get bigint e and n here:
  std::ifstream keyfile;
  keyfile.open(privkey.c_str());
  
  if (!keyfile.is_open()){

    std::cerr << "File '" << privkey <<"' failed to open\n";
    return;
  }
  
  std::string e_str, n_str;
 
  keyfile >> e_str;
  keyfile >> n_str;

  BigUnsigned e = stringToBigUnsigned(e_str);
  BigUnsigned n = stringToBigUnsigned(n_str);

  std::ifstream infile(msgname.c_str(), std::ios::binary);
  std::streampos begin, end;
    begin = infile.tellg();

    infile.seekg(0, std::ios::end);
    end = infile.tellg();

  std::streampos fsize = end - begin;

  infile.seekg(0, std::ios::beg);

  char* memblock = new char[fsize];
  infile.read(memblock, fsize);
  memblock[fsize] = '\0'; // Add end of file
  infile.close();

  //Sign the file!

  std::ofstream outfile((msgname + ".signed").c_str(), std::ios::binary);
  outfile.write(memblock, fsize);
  
  char * signature = new char[SIGLENGTH];

  std::string sig_str = sha256(std::string(memblock));
  BigInteger sig_dec = to_dec(sig_str); 
  
  /* ====MATH AREA===== 
    VARIABLES:
      n: the modulus.
      e: the exponent.

      sig_dec: The base M in decimal.
  */

  BigUnsigned signedMsgInt = modexp(sig_dec,e,n);

  

  std::string signedstring = bigUnsignedToString(signedMsgInt);
  strcpy(signature, signedstring.c_str());
  outfile.write(signature,SIGLENGTH);

  outfile.close();

  return;
}

bool verify(std::string msgname, std::string pubkey){
  

  std::ifstream keyfile;
  keyfile.open(pubkey.c_str());
  // *******************************
  if (!keyfile.is_open()){

    std::cerr << "File '" << pubkey <<"' failed to open\n";
    return false;
  }
  
  std::string d_str, n_str;

  keyfile >> d_str;
  keyfile >> n_str;

  BigUnsigned d = stringToBigUnsigned(d_str);
  BigUnsigned n = stringToBigUnsigned(n_str);

  std::ifstream infile(msgname.c_str(), std::ios::binary);
  std::streampos begin, end;
    begin = infile.tellg();

    infile.seekg(0, std::ios::end);
    end = infile.tellg();

  std::streampos fsize = end - begin - SIGLENGTH;

  infile.seekg(0, std::ios::beg);

  /* Original text */
  char* memblock = new char[fsize];
  infile.read(memblock, fsize);
  memblock[fsize] = '\0'; // Add end of file

  /* Get the signature */
  char * signature = new char[SIGLENGTH];
  infile.read(signature, SIGLENGTH);
  infile.close();

  /* Compare against sha256 of original text */
  std::string sha_str = sha256(std::string(memblock));
  BigInteger sha_dec = to_dec(sha_str);

  /* Convert signature to Bigint so we can verify it */
  BigInteger sint = stringToBigInteger(std::string(signature));
 
  /* ===== MATH AREA =====
    VARIABLES:
      e: the exponent.
      n: The modulus.

      sint: Signed integer(base).

      We want to get the original sha_dec after this transformation.
  */ 
   
  BigUnsigned sig_dec = modexp(sint, d, n);

  return (BigInteger(sig_dec, BigInteger::positive) == sha_dec);
}

// Convert from a string hexidecimal to a Biginteger decimal number.
BigInteger to_dec(std::string hex){

  int hexsize = hex.length();
  BigInteger dec(0);

  BigInteger powerof16(1);

  for (int hd_i = hexsize - 1; hd_i >= 0; --hd_i){ // hd_i hexdigit_index

    char hd = hex[hd_i];
    BigInteger digit(0);    
    

    if (hd == 'a')
      digit = BigInteger(10);
  
    if (hd == 'b')
      digit = BigInteger(11);

    if (hd == 'c')
      digit = BigInteger(12);

    if (hd == 'd')
      digit = BigInteger(13);

    if (hd == 'e')
      digit = BigInteger(14);

    if (hd == 'f')
      digit = BigInteger(15);

    if (digit == 0)
      digit = BigInteger(hd - '0');

    dec += powerof16 * digit;

    powerof16 *= BigInteger(16);
  }

  return dec;

}

    
