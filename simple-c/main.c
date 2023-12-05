#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#include "../includes/config.h"
#include "md4.h"
#include <sys/types.h>

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z) (((x) ^ (y)) ^ (z))
#define H2(x, y, z) ((x) ^ ((y) ^ (z)))

#define STEP(f, a, b, c, d, x, add, s)      \
  (a) += f((b), (c), (d)) + ptr[x] + (add); \
  (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));

void mybody(unsigned char *buffer, uint32_t *result,uint32_t a,uint32_t b,uint32_t c,uint32_t d)
{
  
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
  result[1] = b;
  result[2] = c;
  result[3] = d;
}

void print_hex(const unsigned char *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}



int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s HASH\n", argv[0]);
    return -1;
  }
  unsigned char *target = parse_hash(argv[1]);
  //char *candidate = malloc(PWD_LEN + 1);
  
  char *candidate=malloc(64);
  memset(candidate, 0, 64);
  candidate[56]=PWD_LEN*8;
  memset(candidate, '!', PWD_LEN);
  candidate[PWD_LEN]=0x80;
 
  
  
  //memset(candidate, '!', PWD_LEN);
  //candidate[PWD_LEN] = 0;
  size_t tested = 0;
  struct timeval tval;
  double start;
  double now;

  gettimeofday(&tval, NULL);
  start = tval.tv_sec + tval.tv_usec / 1000000.0;
  print_hex(candidate,64);
  
  
  do {
    
    
    uint32_t res[4];
    
    mybody(candidate,res,0x67452301,0xefcdab89,0x98badcfe,0x10325476);
    tested++;
    if (memcmp(res, target, 16) == 0) {
      candidate[PWD_LEN]=0;
      printf("found: %s, after %ld tries\n", candidate, tested);
      return 0;
    }
    if (tested % (1024 * 1024 * 32) == 0) {
      gettimeofday(&tval, NULL);
      now = tval.tv_sec + tval.tv_usec / 1000000.0;
      double speed = tested / (now - start);
      fprintf(stderr, "%.3f M/s\n", speed / 1000000.0);
    }
  } while (incr_candidate(candidate));
  printf("not found after %ld tries\n", tested);
  
  return 1;
}