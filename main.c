#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

unsigned long long  int e1, e2;
unsigned long long  int p, d;
unsigned long long  int C1, C2;
FILE *out1, *out2;

//fast exponentiation function
unsigned long long  int FastExponention(unsigned long long  int bit, unsigned long long  int n, \
                                        unsigned long long int* y, unsigned long long int* a)
{
    if (bit == 1) {
        *y = (*y * (*a)) % n;
    }
    *a = (*a) * (*a) % n;
}
// Returns gcd of a and b
unsigned long long gcd(unsigned long long  a, unsigned long long  h)
{
	unsigned long long  temp;
	while (1)
	{
	    //a mod h
		temp = a -(a/h)*h;
		if (temp == 0)
		return h;
		a = h;
		h = temp;
	}
}
//Pre-proc for exponentiation
unsigned long long  int Bin_ModExp(unsigned long long  int a, unsigned long long  int m, unsigned long long  int n)
{
    unsigned long long  int r;
    unsigned long long  int y = 1;

    while (m > 0)
    {
        r = m % 2;
        FastExponention(r, n, &y, &a);
        m = m / 2;
    }
    return y;
}
//Key generation algo
unsigned long long  int KeyGeneration()
{
    //Choose prime numbers p and e1
    p = 379;
    e1 = 360;
    d = 67;//dummy value
    do {//private key
        d = rand() % (p - 2) + 1;        //1 <= d <= p-2
    } while (gcd(d, p) != 1);            //d has to be a member of Zp group
    //public key
    e2 = Bin_ModExp(e1, d, p);
    printf("Values of e1 is %d e2 is %d d is %d p is %d\n",e1,e2,d,p);
}
//Elgamal Decryption
unsigned long long  int Decryption(unsigned long long  int C1, unsigned long long  int C2)
{
    FILE* out = fopen("result.txt", "a+");
    //c1^p-1 = 1; c1^-d =c1^p-1-d;
    unsigned long long  int decipher = C2 * Bin_ModExp(C1, p - 1 - d, p) % p;
    fprintf(out, "%c", decipher);
    fclose(out);
}
//Elgamal Encryption
unsigned long long  int Encryption(unsigned long long  int Plaintext)
{
    out1 = fopen("cipher_text1.txt", "a+");
    out2 = fopen("cipher_text2.txt", "a+");

    unsigned long long  int r;
    do {
        r = rand() % (p - 1) + 1;        // 1 < r < p
    }
    while (gcd(r, p) != 1);

    C1 = Bin_ModExp(e1, r, p);
    C2 = Bin_ModExp(e2, r, p) * Plaintext % p;
    //printf("Values of r is %d\n",r);
    fprintf(out1, "%d ", C1);
    fprintf(out2, "%d ", C2);
    fclose(out1);
    fclose(out2);
}

unsigned long long  int main()
{
    FILE *out, *inp;
    // delete contents of these files (from previous runs, if any)
    out = fopen("result.txt", "w+");
    fclose(out);
    out = fopen("cipher_text1.txt", "w+");
    fclose(out);
    out = fopen("cipher_text2.txt", "w+");
    fclose(out);

    KeyGeneration();
    printf("Processing the file...... \n");

    inp = fopen("plain_text.txt", "r+");
    if (inp == NULL)
    {
        printf("Error opening Source File\n");
        exit(1);
    }

    while (1)
    {
        char ch = getc(inp);
        if (ch == EOF) {
            break;            // M < p
        }
        else if (ch > p) {
            printf("Error This character cannot be Encrypted as it is greater than p: ch is %d p is %llu\n", ch,d);
        }
        Encryption(toascii(ch));
    }

    fclose(inp);
    FILE *inp1, *inp2;
    inp1 = fopen("cipher_text1.txt", "r");
    inp2 = fopen("cipher_text2.txt", "r");

    long long int C1, C2;
    while (1)
    {
        unsigned long long  int ret = fscanf(inp1, "%d", &C1);
        fscanf(inp2, "%d", &C2);
        if (ret == -1) {
            break;
        }

        Decryption(C1, C2);
    }
    printf("Plain Text input for Encryption is taken from plantext.txt\n");
    printf("Encryption output is written to cipher_text1.txt, cipher_text2.txt\n");
    printf("Decryption output is written to result.txt\n");

    fclose(inp1);
    fclose(inp2);
    return 0;
    getch();
}
