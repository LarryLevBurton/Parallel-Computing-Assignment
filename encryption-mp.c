#include "encryption.h"

#define SECOND_DIVIDER 60000
#define NUM_THREADS 4


int main (void)
{

  /* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */
  /* A 128 bit key */
  unsigned char *key = (unsigned char *)"#####700000#####";
  // unsigned char *key = (unsigned char *)"#####969249#####";
  // unsigned char *key = (unsigned char *)"0123456789012345";
  unsigned char decKey[17] = "#####000000#####";
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

  /* Do something usefulgcc  with the ciphertext here */
  printf("Ciphertext is:\n");
  BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  int flag = 0;
  omp_set_num_threads(NUM_THREADS);
  omp_lock_t lock;
  omp_init_lock(&lock);

  double start = omp_get_wtime();
  #pragma omp parallel 
  {
    unsigned char ciphertext2[128];
    unsigned char key2[17];
    omp_set_lock(&lock);
      strcpy(ciphertext2,ciphertext);
      strcpy(key2,key);
      int ciphertext_len2 = ciphertext_len;
    omp_unset_lock(&lock);
    // unsigned char *decKey2[17] = decKey;
   flag = generateWord(key2,&flag, &lock,ciphertext2,ciphertext_len2);
  }

  printTime(start);
 

  return 0;
}


int generateWord(unsigned char *key, int *flag, omp_lock_t *lock
                ,unsigned char ciphertext[128],int ciphertext_len){
  int id = omp_get_thread_num();
  // int otherID = id+1;
  // printf("id: %d\n", id);

  
  unsigned char TempDecKey[17] = "#####000000#####";
  static char alpha[26] = "abcdef0123456789";

  int alphaLength = getLength(alpha);
  for (int pos0 = id; pos0 < alphaLength; pos0 +=NUM_THREADS) {
    TempDecKey[5] = alpha[pos0]; 
    for (int pos1 = 0; pos1 < alphaLength; pos1++) {
        TempDecKey[6] = alpha[pos1]; 
        for (int pos2 = 0; pos2 < alphaLength; pos2++) {
          TempDecKey[7] = alpha[pos2]; 
          for (int pos3 = 0; pos3 < alphaLength; pos3++) {
            TempDecKey[8] = alpha[pos3]; 
            for (int pos4 = 0; pos4 < alphaLength; pos4++) {
              TempDecKey[9] = alpha[pos4]; 
              for (int pos5 = 0; pos5 < alphaLength; pos5++) {
                TempDecKey[10] = alpha[pos5]; 
                  if(strncmp(key,TempDecKey,getLength(TempDecKey)) == 0 ){
                    printf("\nKey Cracked: %s : %s\n\n", key, TempDecKey);
                    decFunction(ciphertext, ciphertext_len,TempDecKey);
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

