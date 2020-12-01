#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdbool.h>

void handleErrors(void);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);
char *inputString(FILE* fp, size_t size);

int main (void)
{
    /*
     * Set up the key and iv.
     */

    /* A 256 bit key */
    unsigned char key[32];
    RAND_bytes(key, sizeof(key));
    printf("The key is:\n");
    BIO_dump_fp (stdout, (const char *)key, 256);

    /* A 256 bit IV */
    unsigned char iv[32];

    bool exit_loop = false;
    while(!exit_loop) {
        char exit_string[] = "exit program";
        unsigned char *plaintext;
        printf("Enter your message:\n");
        plaintext = inputString(stdin, 10);
        printf("You have entered:\n %s\n", plaintext);
        if(strcmp(plaintext, exit_string) != 0){
            RAND_bytes(iv, sizeof(iv));
            printf("The iv is:\n");
            BIO_dump_fp (stdout, (const char *)iv, 256);
                /* Message to be encrypted */
            size_t plaintext_size = strlen(plaintext) * 8;
            printf("Size of plaintext is: %ld bits\n", plaintext_size);   

            /*
            * Buffer for ciphertext. Ensure the buffer is long enough for the
            * ciphertext which may be longer than the plaintext, depending on the
            * algorithm and mode.
            */
            unsigned char ciphertext[plaintext_size];

            /* Buffer for the decrypted text */
            unsigned char decryptedtext[plaintext_size];

            int decryptedtext_len, ciphertext_len;

            /* Encrypt the plaintext */
            ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                                    ciphertext);

            /* Do something useful with the ciphertext here */
            printf("Ciphertext is:\n");
            BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

            /* Decrypt the ciphertext */
            decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                        decryptedtext);

            /* Add a NULL terminator. We are expecting printable text */
            decryptedtext[decryptedtext_len] = '\0';

            /* Show the decrypted text */
            printf("Decrypted text is:\n");
            printf("%s\n", decryptedtext);

            free(plaintext);
        }
        else {
            exit_loop = true;
        }

    }


    return 0;
}




void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
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
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}
