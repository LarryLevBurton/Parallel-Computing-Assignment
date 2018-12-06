#include "encryption.h"

#define SECOND_DIVIDER 60000
#define NUM_THREADS 4

/*Description:This is the main function for the parellel brute force. 
              it will encrypt/decrypt a string using the openssl library and a 
              128 bit key. It will brute force your the key that was used 
              to encrypt string. The key is padded by five # either side. 
              This use MPI with four threads to run in parellel.

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
  int decryptedtext_len, ciphertext_len, bruteCiphertext_len;

  /* Encrypt the plaintext */
  ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,ciphertext);
  
  /*Print Cipher Text*/ 
  printf("Ciphertext is:\n");
  BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);


  int flag = 0;
  /* Specify number of threads*/
  omp_set_num_threads(NUM_THREADS);

  /*Create OMP Lock*/
  omp_lock_t lock;
  omp_init_lock(&lock);

  /* Starts timer*/
  double start = omp_get_wtime();



  /*Start of Parellel Section*/
  #pragma omp parallel 
  {
    /*Create copy of variables.*/
    unsigned char ciphertext2[128];
    unsigned char key2[17];
    /*Enable lock to allow variables to be acessed without anything changing*/
    omp_set_lock(&lock);
      strcpy(ciphertext2,ciphertext);
      strcpy(key2,key);
      int ciphertext_len2 = ciphertext_len;
    omp_unset_lock(&lock);

    /*Call Function to start brute forcec*/
   flag = generateWord(key2,&flag, &lock,ciphertext2,ciphertext_len2);
  }

  printTime(start);
 

  return 0;
}



/*Description:This is function will keep genarting six character keys
              until it's either found a match to or there is no more 
              options. 

	Parameters: unsigned char *key - Original key used
              int *flag - pointer to finish flag
              omp_lock_t *lock - pointer to OMP lock
              unsigned char ciphertext[128] - cycphertext
              int ciphertext_len - length of cyphertext

	Returns: it will return 1 if sucessful and 0 if it fails
	
	Author(s): Laurence Lev Burton

*/
int generateWord(unsigned char *key, int *flag, omp_lock_t *lock
                ,unsigned char ciphertext[128],int ciphertext_len){
  int id = omp_get_thread_num();

  /* Key used for brute force with five # each 
  * side for padding to make the programe 
  * run faster
  */
  unsigned char decKey[17] = "#####000000#####";

  /* Characters used for brute force*/
  static char alpha[26] = "abcdef0123456789";

  int alphaLength = getLength(alpha);

/*Start of generating words */
  for (int pos0 = id; pos0 < alphaLength; pos0 +=NUM_THREADS) {
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
                  if(strncmp(key,decKey,getLength(decKey)) == 0 ){
                    printf("\nKey Cracked: %s : %s\n\n", key, decKey);
                    decFunction(ciphertext, ciphertext_len,decKey);
                    return 1;
                  }else if(*flag == 1){
                    return 1; 
                  };
              }
          }
        }
      }
    }
  }

  return 0;


}

