#include <mpi.h>
#include <stdio.h>
#include "encryption.h"
#include <unistd.h>

int BruteForce(unsigned char *key, int *lock
                ,unsigned char ciphertext[128],int ciphertext_len,int rank, int size,double start);

int main(int argc, char *argv[]){



    char hostname[1024];
    gethostname(hostname, 1024);

    int rank,size,i,j,err;
    int flag = 0;
    int lock = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    /* A 128 bit key */
    unsigned char *key = (unsigned char *)"#####765432#####";
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

    /* Checks if ID is one and prints out the ciphertext.*/

    if(rank == 0){
        printf("Ciphertext is:\n");
        BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
    }


    double start = omp_get_wtime();
    
    flag = BruteForce(key, &lock,ciphertext,ciphertext_len,rank,size, start);
    MPI_Finalize();

    return 0;

}
 
int BruteForce(unsigned char *key, int *lock
                ,unsigned char ciphertext[128],int ciphertext_len,int rank, int size,double start){

    int flag = 0;
    int otherflag = 0;
    // printf("Rank: %d Size: %d\n", rank, size);
    if(size = 0)rank = 1;

    unsigned char TempDecKey[17] = "#####000000#####";
    static char alpha[26] = "abcdef0123456789";

    MPI_Barrier(MPI_COMM_WORLD);

    int alphaLength = getLength(alpha);
    for (int pos0 = size; pos0 < alphaLength; pos0 = pos0 + rank) {
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
                                printf("Rank :%d size: %d \n",size,rank);
                                printf("\nKey Cracked: %s : %s\n\n", key, TempDecKey);
                                decFunction(ciphertext, ciphertext_len,TempDecKey);
                                printTime(start);
                                MPI_Abort(MPI_COMM_WORLD,0);
                                return 1;
                            } 
                        }
                    }
                }
            }
        }
    }

    return 0;
}


