#include <stdio.h>
#include <emmintrin.h>
#include "../includes/config.h"
#include <stdint.h>
#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z) (((x) ^ (y)) ^ (z))
#define H2(x, y, z) ((x) ^ ((y) ^ (z)))

#define NUM_M128 16
#define NUM_BUFFER 4
#define PWD_LEN 6

#define STEP(f, a, b, c, d, x, add, s)      \
  (a) += f((b), (c), (d)) + ptr[x] + (add); \
  (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));

void mybody(unsigned char *buffer, unsigned char *result)
{
    uint32_t a = 0x67452301;
  uint32_t b = 0xefcdab89;
  uint32_t c = 0x98badcfe;
  uint32_t d = 0x10325476;
  uint32_t *ptr = buffer;
  STEP(F, a, b, c, d, 0, 0, 3)
  STEP(F, d, a, b, c, 1, 0, 7)
  STEP(F, c, d, a, b, 2, 0, 11)
  STEP(F, b, c, d, a, 3, 0, 19)
  STEP(F, a, b, c, d, 4, 0, 3)
  STEP(F, d, a, b, c, 5, 0, 7)
  STEP(F, c, d, a, b, 6, 0, 11)
  STEP(F, b, c, d, a, 7, 0, 19)
  STEP(F, a, b, c, d, 8, 0, 3)
  STEP(F, d, a, b, c, 9, 0, 7)
  STEP(F, c, d, a, b, 10, 0, 11)
  STEP(F, b, c, d, a, 11, 0, 19)
  STEP(F, a, b, c, d, 12, 0, 3)
  STEP(F, d, a, b, c, 13, 0, 7)
  STEP(F, c, d, a, b, 14, 0, 11)
  STEP(F, b, c, d, a, 15, 0, 19)

  /* Round 2 */
  STEP(G, a, b, c, d, 0, 0x5a827999, 3)
  STEP(G, d, a, b, c, 4, 0x5a827999, 5)
  STEP(G, c, d, a, b, 8, 0x5a827999, 9)
  STEP(G, b, c, d, a, 12, 0x5a827999, 13)
  STEP(G, a, b, c, d, 1, 0x5a827999, 3)
  STEP(G, d, a, b, c, 5, 0x5a827999, 5)
  STEP(G, c, d, a, b, 9, 0x5a827999, 9)
  STEP(G, b, c, d, a, 13, 0x5a827999, 13)
  STEP(G, a, b, c, d, 2, 0x5a827999, 3)
  STEP(G, d, a, b, c, 6, 0x5a827999, 5)
  STEP(G, c, d, a, b, 10, 0x5a827999, 9)
  STEP(G, b, c, d, a, 14, 0x5a827999, 13)
  STEP(G, a, b, c, d, 3, 0x5a827999, 3)
  STEP(G, d, a, b, c, 7, 0x5a827999, 5)
  STEP(G, c, d, a, b, 11, 0x5a827999, 9)
  STEP(G, b, c, d, a, 15, 0x5a827999, 13)

  /* Round 3 */
  STEP(H, a, b, c, d, 0, 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, 8, 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, 4, 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, 12, 0x6ed9eba1, 15)
  STEP(H, a, b, c, d, 2, 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, 10, 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, 6, 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, 14, 0x6ed9eba1, 15)
  STEP(H, a, b, c, d, 1, 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, 9, 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, 5, 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, 13, 0x6ed9eba1, 15)
  STEP(H, a, b, c, d, 3, 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, 11, 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, 7, 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, 15, 0x6ed9eba1, 15)
  a += 0x67452301;
  b += 0xefcdab89;
  c += 0x98badcfe;
  d += 0x10325476;
  result[0] = a;
	result[1] = a >> 8;
	result[2] = a >> 16;
	result[3] = a >> 24;
	result[4] = b;
	result[5] = b >> 8;
	result[6] = b >> 16;
	result[7] = b >> 24;
	result[8] = c;
	result[9] = c >> 8;
	result[10] = c >> 16;
	result[11] = c >> 24;
	result[12] = d;
	result[13] = d >> 8;
	result[14] = d >> 16;
	result[15] = d >> 24;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s HASH\n", argv[0]);
    return -1;
  }
  
  char **bufferArray = malloc(NUM_BUFFER * sizeof(char*));
  for (int i = 0; i < NUM_BUFFER; i++) {
        bufferArray[i] = malloc(64);

        }
  char *candidate1=malloc(64);
  memset(candidate1, 0, 64);
  candidate1[56]=PWD_LEN*8;
  memset(candidate1, '!', PWD_LEN);
  candidate1[PWD_LEN]=0x80;
  for(int i=0;i<NUM_BUFFER;i++){
    for(int j=0;j<64;j++){
      
      bufferArray[i][j]=candidate1[j];
    }
    printf("\n");
    incr_candidate(candidate1);
  }
  for(int i=0;i<NUM_BUFFER;i++){
    for(int j=0;j<64;j++){
      printf("%02x", (unsigned char)bufferArray[i][j]);
      
    }
    printf("\n");
    
  }
   do {
    
    
    unsigned char res[16];
    for(int i=0;i<NUM_BUFFER;i++){
    for(int j=0;j<64;j++){
    bufferArray[i][j]=candidate1[j];
      printf("%02x", (unsigned char)bufferArray[i][j]);
      
      
    }
    incr_candidate(candidate1);
    printf("\n");
    
  }
    
    
    
    
  } while (incr_candidate(bufferArray[0]));
  
  
    
    

}
