#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

int generateWord(unsigned char *key, int *flag, omp_lock_t *lock
                ,unsigned char ciphertext[128],int ciphertext_len);
int decFunction( unsigned char ciphertext[128],int ciphertext_len, unsigned char *decKey);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext);

// int getLength(char *decKey);
// int printTime(clock_t time);


int getLength(char *decKey){
  int count;
  for(count = 0; decKey[count] != '\0'; count++){}
  return count;

}
int printTime(double start){
  double end = omp_get_wtime();
  double time_spent = (double)(end - start);
  printf("%f \n", time_spent);  return 0;

}



int decFunction(  unsigned char ciphertext[128],int ciphertext_len, unsigned char *decKey){
 
  /* Buffer for the decrypted text */
  unsigned char decryptedtext[128];
  unsigned char *iv = (unsigned char *)"012345678912343";
  int decryptedtext_len;

  /* Decrypt the ciphertext */
  // decryptedtext_len = decrypt(ciphertext, ciphertext_len, decKey, iv, decryptedtext);
  decryptedtext_len = decrypt(ciphertext, ciphertext_len, decKey, iv, decryptedtext);

  /* Add a NULL terminator. We are expecting printable text */
  decryptedtext[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  printf("Decrypted text is:\n\n%s\n\n", decryptedtext);
}

void handleErrors(void)
{
  printf("\n\n");
//   printf("%s",*stderr);
  ERR_print_errors_fp(stderr);
  printf("\n\n");
//   abort();
}


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;
  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
  return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;
  int len;
  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we#define SECOND_DIVIDER 100000
 are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
  return plaintext_len;
}
