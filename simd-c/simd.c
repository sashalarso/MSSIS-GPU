#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <emmintrin.h>

#include "../includes/config.h"

#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			(((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z)			(((x) ^ (y)) ^ (z))
#define H2(x, y, z)			((x) ^ ((y) ^ (z)))

#define STEP(f, a, b, c, d, x, add, s)      \
  (a) = _mm_add_epi32(_mm_add_epi32(a,f((b), (c), (d))), _mm_add_epi32(ptr[x],(add))); \
  (a) = ((_mm_slli_epi32((a),(s))) | (_mm_srli_epi32(((a) & 0xffffffff),(32 - (s)))));

int MD4_Super(unsigned char *target, const void  *data)
{

  __m128i * ptr = data;

  __m128i a, b, c, d;
	a = _mm_set1_epi32(0x67452301);
	b = _mm_set1_epi32(0xefcdab89);
	c = _mm_set1_epi32(0x98badcfe);
	d = _mm_set1_epi32(0x10325476);

  __m128i add_ = _mm_set1_epi32(0);
  /*1*/
	STEP(F, a, b, c, d, 0,add_, 3)
	STEP(F, d, a, b, c, 1,add_, 7)
	STEP(F, c, d, a, b, 2,add_, 11)
	STEP(F, b, c, d, a, 3,add_, 19)
	STEP(F, a, b, c, d, 4,add_, 3)
	STEP(F, d, a, b, c, 5,add_, 7)
	STEP(F, c, d, a, b, 6,add_, 11)
	STEP(F, b, c, d, a, 7,add_, 19)
	STEP(F, a, b, c, d, 8,add_, 3)
	STEP(F, d, a, b, c, 9,add_, 7)
	STEP(F, c, d, a, b, 10,add_, 11)
	STEP(F, b, c, d, a, 11,add_, 19)
	STEP(F, a, b, c, d, 12,add_, 3)
	STEP(F, d, a, b, c, 13,add_, 7)
	STEP(F, c, d, a, b, 14,add_, 11)
	STEP(F, b, c, d, a, 15,add_, 19)
  /*2*/

  add_ = _mm_set1_epi32(0x5a827999);
	STEP(G, a, b, c, d, 0, add_, 3)
	STEP(G, d, a, b, c, 4,  add_, 5)
  STEP(G, c, d, a, b, 8,  add_, 9)
	STEP(G, b, c, d, a, 12,  add_, 13)
	STEP(G, a, b, c, d, 1,  add_, 3)
	STEP(G, d, a, b, c, 5,  add_, 5)
	STEP(G, c, d, a, b, 9,  add_, 9)
	STEP(G, b, c, d, a, 13, add_, 13)
	STEP(G, a, b, c, d, 2,  add_, 3)
	STEP(G, d, a, b, c, 6,  add_, 5)
	STEP(G, c, d, a, b, 10,  add_, 9)
	STEP(G, b, c, d, a, 14,  add_, 13)
	STEP(G, a, b, c, d, 3,  add_, 3)
	STEP(G, d, a, b, c, 7,  add_, 5)
	STEP(G, c, d, a, b, 11,  add_, 9)
	STEP(G, b, c, d, a, 15,  add_, 13)
  /*3*/
   add_ = _mm_set1_epi32(0x6ed9eba1);
	STEP(H, a, b, c, d, 0, add_, 3)
  STEP(H2, d, a, b, c, 8, add_, 9)
  STEP(H, c, d, a, b,  4, add_, 11)
	STEP(H2, b, c, d, a, 12, add_, 15)
	STEP(H, a, b, c, d, 2, add_, 3)
	STEP(H2, d, a, b, c, 10, add_, 9)
	STEP(H, c, d, a, b, 6, add_, 11)
	STEP(H2, b, c, d, a, 14, add_, 15)
	STEP(H, a, b, c, d, 1, add_, 3)
	STEP(H2, d, a, b, c, 9, add_, 9)
	STEP(H, c, d, a, b, 5, add_, 11)
	STEP(H2, b, c, d, a, 13, add_, 15)
	STEP(H, a, b, c, d, 3, add_, 3)
	STEP(H2, d, a, b, c, 11, add_, 9)
	STEP(H, c, d, a, b, 7, add_, 11)
	STEP(H2, b, c, d, a, 15, add_, 15)

	a = _mm_add_epi32(a,_mm_set1_epi32(0x67452301));
	b = _mm_add_epi32(b,_mm_set1_epi32(0xefcdab89));
	c = _mm_add_epi32(c,_mm_set1_epi32(0x98badcfe));
	d = _mm_add_epi32(d,_mm_set1_epi32(0x10325476));

  uint32_t * target_tmp = target;

  __m128i result_a = _mm_cmpeq_epi32(a,_mm_set1_epi32(target_tmp[0]));
  __m128i result_b = _mm_cmpeq_epi32(b,_mm_set1_epi32(target_tmp[1]));
  __m128i result_c = _mm_cmpeq_epi32(c,_mm_set1_epi32(target_tmp[2]));
  __m128i result_d = _mm_cmpeq_epi32(d,_mm_set1_epi32(target_tmp[3]));

  __m128i result = result_a & result_b & result_c & result_d;
  int res = _mm_movemask_epi8(result);
  if(res>0){
    printf("Hash found %d\n",res);
    if(res == 61440) return 3;
    if(res == 3840) return 2;
    if(res == 240) return 1;
    if(res == 15) return 0;
  }
  return -1;

}

int main(int argc, char **argv){
  printf("ici");
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s HASH\n", argv[0]);
    return -1;
  }
  printf("ici");
  unsigned char *target = parse_hash(argv[1]);
  char *candidate = malloc(8);
  memset(candidate,0,8);
  memset(candidate, '!', PWD_LEN);
  candidate[PWD_LEN] = 0x80;
  unsigned char *buffer = malloc(256);
  memset(buffer, 0, 256);
  buffer[224]=PWD_LEN*8;
  buffer[228]=PWD_LEN*8;
  buffer[232]=PWD_LEN*8;
  buffer[236]=PWD_LEN*8;

  size_t tested = 0;
  struct timeval tval;
  double start;
  double now;

  gettimeofday(&tval, NULL);
  start = tval.tv_sec + tval.tv_usec / 1000000.0;

  do{
    for(int i =0; i<4;i++){
      for (int j = 0; j < 4; j++) {
        buffer[i * 4 + j] = candidate[j];
        buffer[16 + i * 4 + j] = candidate[j + 4];
      }
      incr_candidate(candidate);
    }

    unsigned char res[64];
    int i = MD4_Super(target,buffer);
    tested+=4;
    if (i>=0) {
      for (int j = 0; j < 4; j++) {
        candidate[j] = buffer[i * 4 + j];
        if(j+4<PWD_LEN){
          candidate[j + 4] = buffer[16 + i * 4 + j];
        }else{
          candidate[j+4]=0;
        }
        
      }
      printf("found: %s, after %ld tries\n", candidate, tested);
      return 0;
    }
    if (tested % (1024 * 1024 * 32*4) == 0) {
      gettimeofday(&tval, NULL);
      now = tval.tv_sec + tval.tv_usec / 1000000.0;
      double speed = tested / (now - start);
      fprintf(stderr, "%.3f M/s\n", speed / 1000000.0);
    }
  }while(incr_candidate(candidate));
  free(target);
  free(candidate);
  free(buffer);
  return 0;
}
