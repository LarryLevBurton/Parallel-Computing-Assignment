#include <mpi.h>
#include <stdio.h>
#include "encryption.h"
#include <unistd.h>

int BruteForce(unsigned char *key, int *lock
                ,unsigned char ciphertext[128],int ciphertext_len,int rank, int size);

int main(int argc, char *argv[]){



    char hostname[1024];
    gethostname(hostname, 1024);

    printf("Hostname: %s \n",hostname);

    int rank,size,i,j,err;
    int flag = 0;
    int lock = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    /* A 128 bit key */
    unsigned char *key = (unsigned char *)"#####700000#####";
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
    MPI_Barrier(MPI_COMM_WORLD);


    // unsigned char ciphertext2[128];
    // unsigned char key2[17];
    // int ciphertext_len2 = ciphertext_len;

    // omp_set_lock(&lock);
    // strcpy(ciphertext2,ciphertext);
    // strcpy(key2,key);
    // omp_unset_lock(&lock);       
    flag = BruteForce(key, &lock,ciphertext,ciphertext_len,size,rank);
    MPI_Finalize();

    return 0;

}
 
int BruteForce(unsigned char *key, int *lock
                ,unsigned char ciphertext[128],int ciphertext_len,int rank, int size){

    int flag = 0;
    printf("Rank :%d size: %d \n",size,rank);

    unsigned char TempDecKey[17] = "#####000000#####";
    static char alpha[26] = "abcdef0123456789";

    int otherRank = rank++;
    if(otherRank == size){
        otherRank = 0;
    }
    printf("Other Rank: %d", otherRank);

    int alphaLength = getLength(alpha);
    for (int pos0 = size; pos0 < alphaLength; pos0 +=rank) {
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
                            // MPI_Bcast(&flag, 1, MPI_INT, otherRank, MPI_COMM_WORLD);

                            if(strncmp(key,TempDecKey,getLength(TempDecKey)) == 0 ){
                                flag = 1;
                                // printf("1 flag : %d\n",flag);
                                MPI_Bcast(&flag, 1, MPI_INT, rank, MPI_COMM_WORLD);
                                printf("\nKey Cracked: %s : %s\n\n", key, TempDecKey);
                                decFunction(ciphertext, ciphertext_len,TempDecKey);
                                // MPI_Send(&flag, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                                return 1;
 
                            } 
                            // printf("2 flag : %d\n",flag);

                            for(int x = 0; x < size; x++){
                                // printf("X: %d\n",x);
                                if(x != rank){
                                    MPI_Bcast(&flag, 1, MPI_INT, x, MPI_COMM_WORLD);
                                    if(flag == 1){
                                        return 1; 
                                    }
                                }
                            }
                            // MPI_Bcast(void* data,int count,MPI_Datatype datatype,int root,MPI_Comm communicator)
                            // MPI_Recv(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        }
                    }
                }
            }
        }
    }

    return 0;
}


