#ifndef _AES_H_
#define _AES_H_

#include <stdio.h>
#include <string.h>

#define AES_NB 4
#define AES_MAXNK 8
#define AES_MAXNR 14
class AES
{
public:
	typedef enum KEYSIZE_
	{
		BIT128 = 0,
		BIT192,
		BIT256
	}KEYSIZE;

public:
	AES( KEYSIZE keysize, unsigned char *key, int length );
	~AES(void);
	void* Cipher(void* input, int length);
	void* InvCipher(void* input, int length);

protected:
	//int Nb; //=AES_NB
	int Nk;
	int Nr;
	//give max length
	unsigned char key[AES_NB*AES_MAXNK];// the seed key. size will be 4 * keySize from ctor.
	typedef struct BYTE4_
	{
		unsigned char w[4];
	}BYTE4;
	BYTE4 w[AES_NB * (AES_MAXNR+1)];
	unsigned char State[4][AES_NB];

	void RotWord( unsigned char *word );
	void SubWord( unsigned char *word );
	void AddRoundKey(int round);
	void SubBytes();
	void InvSubBytes();
	void ShiftRows();
	void InvShiftRows();
	void MixColumns();
	void InvMixColumns();

	void Cipher( unsigned char *input );
	void InvCipher( unsigned char *input );
	void Cipher_less_4x(void* input, int length);
	void InvCipher_less_4x(void* input, int length);

	static unsigned char gfmultby01(unsigned char b)
    {
      return b;
    }

    static unsigned char gfmultby02(unsigned char b)
    {
      if (b < 0x80)
        return (unsigned char)(int)(b <<1);
      else
        return (unsigned char)( (int)(b << 1) ^ (int)(0x1b) );
    }

    static unsigned char gfmultby03(unsigned char b)
    {
      return (unsigned char) ( (int)gfmultby02(b) ^ (int)b );
    }

    static unsigned char gfmultby09(unsigned char b)
    {
      return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)b );
    }

    static unsigned char gfmultby0b(unsigned char b)
    {
      return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)gfmultby02(b) ^
                     (int)b );
    }

    static unsigned char gfmultby0d(unsigned char b)
    {
      return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)gfmultby02(gfmultby02(b)) ^
                     (int)(b) );
    }

    static unsigned char gfmultby0e(unsigned char b)
    {
      return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                     (int)gfmultby02(gfmultby02(b)) ^
                     (int)gfmultby02(b) );
    }
};

#endif //#ifndef _AES_H_
