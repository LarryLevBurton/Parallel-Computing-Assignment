#include "encryption.h"

#define SECOND_DIVIDER 100000
#define NUM_THREADS 1


int main (void)
{
    // clock_t start = clock();

  /* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */
  /* A 256 bit key */
  unsigned char *key = (unsigned char *)"#####999999#####";

  // unsigned char *key = (unsigned char *)"0123456789012345";
  // unsigned char decKey[17] = "#####000000#####\0";

//   /* A 128 bit IV */
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

  /* Do something usefulgcc  with the ciphertext here */
  printf("Ciphertext is:\n");
  BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  
  unsigned char decKey[17] = "#####000000#####";
  char alpha[26] = "abcdef0123456789";

  int alphaLength = getLength(alpha);
  double start = omp_get_wtime();

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
                if(strcmp(key,decKey) == 0 ){  
                    printf("\nKey Cracked: %s : %s\n\n", key, decKey);
                    decFunction(ciphertext, ciphertext_len,decKey);
                    printTime(start);
                    return 0;
                  }
              }
          }
        }
      }
    }
  }


  return 0;

}




