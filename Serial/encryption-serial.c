#include "encryption.h"

#define SECOND_DIVIDER 100000
#define NUM_THREADS 1

/*Description:This is the main function for the serial brute force. 
              it will encrypt/decrypt a string using the openssl library and a 
              128 bit key. It will brute force your the key that was used 
              to encrypt string. The key is padded by five # either side.

	Parameters: void


	Returns: it will return 1 if sucessful and 0 if it fails
	
	Author(s): Laurence Lev Burton
  Credit: Matt Caswell(OpenSSl Wiki)
*/
int main (void)
{
  
  /* A 128 bit key */
  unsigned char *key = (unsigned char *)"#####594232#####";

  /* A 128 bit IV */
  unsigned char *iv = (unsigned char *)"012345678912343";

  /* Message to be encrypted */
  unsigned char *plaintext = (unsigned char *)"The quick brown fox jumps over the lazy dog";

  /* Buffer for ciphertext. Ensure the buffer is long enough for the
  * ciphertext which may be longer than the plaintext, dependant on the
  * algorithm and mode
  */
  unsigned char ciphertext[128];

  /* Buffer for the decrypted text */
  unsigned char decryptedtext[128];

  int decryptedtext_len, ciphertext_len, bruteCiphertext_len;

  /* Encrypt the plaintext */
  ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,ciphertext);

  /*Print Cipher Text*/ 
  printf("Ciphertext is:\n");
  BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);


  /* Key used for brute force with five # each 
  * side for padding to make the programe 
  * run faster
  */
  unsigned char decKey[17] = "#####000000#####";

  /* Characters used for brute force*/
  char alpha[26] = "abcdef0123456789";

  /* Gets amount of characters used in the previous array */
  int alphaLength = getLength(alpha);

  /* Starts timer*/
  double start = omp_get_wtime();


/*Start of generating words*/
  for (int pos0 = 0; pos0 < alphaLength; pos0++) {
    decKey[5] = alpha[pos0]; 
    for (int pos1 = 0; pos1 < alphaLength; pos1++) {
        decKey[6] = alpha[pos1]; 
        for (int pos2 = 0; pos2 < alphaLength; pos2++) {
          decKey[7] = alpha[pos2]; 
          for (int pos3 = 0; pos3 < alphaLength; pos3++) {
            decKey[8] = alpha[pos3]; 
            for (int pos4 = 0; pos4 < alphaLength; pos4++) {
              decKey[9] = alpha[pos4]; 
              for (int pos5 = 0; pos5 < alphaLength; pos5++) {
                decKey[10] = alpha[pos5];
                /* Compares generated key to previously made key.*/
                if(strcmp(key,decKey) == 0 ){  
                    printf("\nKey Cracked: %s : %s\n\n", key, decKey);
                    decFunction(ciphertext, ciphertext_len,decKey);
                    printTime(start);
                    return 1;
                  }
              }
          }
        }
      }
    }
  }
/*End of brute force*/

  return 0;

}




