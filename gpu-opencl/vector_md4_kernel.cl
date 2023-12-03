#define PWD_LEN 7
/* LES 2 premir carac du mdp sont des lettres de l'alphabet et ceux d'après peuvent être de scarac spéciaux*/

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z) (((x) ^ (y)) ^ (z))
#define H2(x, y, z) ((x) ^ ((y) ^ (z)))

/*
 * The MD4 transformation for all three rounds.
 */
#define STEP(f, a, b, c, d, x, s) \
  (a) += f((b), (c), (d)) + (x);  \
  (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));

#define SET(x) ptr[(x)]
#define GET(x) ptr[(x)]

int compare(const unsigned char *s1, const unsigned char *s2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            return 1;
        }
    }
    return 0;
}

int incr_candidate(char *ptr)
{
  size_t pos = PWD_LEN - 1;
  while (1)
  {
    if (pos < 3)
    {
      return 0;
    }
    char c = ++ptr[pos];
    if (c == '\'')
    {
      ptr[pos] = 'a';
      return 1;
    }
    if (c <= '&')
      return 1;
    if (c > 'z')
    {
      ptr[pos] = '!';
      pos--;
    } else {
      return 1;
    }
  }
}

void mybody(unsigned char *candidate, uint *result)
{
  uint a, b, c, d;
  a = 0x67452301;
  b = 0xefcdab89;
  c = 0x98badcfe;
  d = 0x10325476;
  uint *ptr = candidate;

  STEP(F, a, b, c, d, SET(0), 3)
  STEP(F, d, a, b, c, SET(1), 7) 

  STEP(F, c, d, a, b, SET(2), 11)
  STEP(F, b, c, d, a, SET(3), 19)
  STEP(F, a, b, c, d, SET(4), 3)
  STEP(F, d, a, b, c, SET(5), 7)
  STEP(F, c, d, a, b, SET(6), 11)
  STEP(F, b, c, d, a, SET(7), 19)
  STEP(F, a, b, c, d, SET(8), 3)
  STEP(F, d, a, b, c, SET(9), 7)
  STEP(F, c, d, a, b, SET(10), 11)
  STEP(F, b, c, d, a, SET(11), 19)
  STEP(F, a, b, c, d, SET(12), 3)
  STEP(F, d, a, b, c, SET(13), 7)
  STEP(F, c, d, a, b, SET(14), 11)
  STEP(F, b, c, d, a, SET(15), 19)

  // Round 2
  STEP(G, a, b, c, d, GET(0) + 0x5a827999, 3)
  STEP(G, d, a, b, c, GET(4) + 0x5a827999, 5)
  STEP(G, c, d, a, b, GET(8) + 0x5a827999, 9)
  STEP(G, b, c, d, a, GET(12) + 0x5a827999, 13)
  STEP(G, a, b, c, d, GET(1) + 0x5a827999, 3)
  STEP(G, d, a, b, c, GET(5) + 0x5a827999, 5)
  STEP(G, c, d, a, b, GET(9) + 0x5a827999, 9)
  STEP(G, b, c, d, a, GET(13) + 0x5a827999, 13)
  STEP(G, a, b, c, d, GET(2) + 0x5a827999, 3)
  STEP(G, d, a, b, c, GET(6) + 0x5a827999, 5)
  STEP(G, c, d, a, b, GET(10) + 0x5a827999, 9)
  STEP(G, b, c, d, a, GET(14) + 0x5a827999, 13)
  STEP(G, a, b, c, d, GET(3) + 0x5a827999, 3)
  STEP(G, d, a, b, c, GET(7) + 0x5a827999, 5)
  STEP(G, c, d, a, b, GET(11) + 0x5a827999, 9)
  STEP(G, b, c, d, a, GET(15) + 0x5a827999, 13)

  // Round 3
  STEP(H, a, b, c, d, GET(0) + 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, GET(8) + 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, GET(4) + 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, GET(12) + 0x6ed9eba1, 15)
  STEP(H, a, b, c, d, GET(2) + 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, GET(10) + 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, GET(6) + 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, GET(14) + 0x6ed9eba1, 15)
  STEP(H, a, b, c, d, GET(1) + 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, GET(9) + 0x6ed9eba1, 9) 
  STEP(H, c, d, a, b, GET(5) + 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, GET(13) + 0x6ed9eba1, 15)
  STEP(H, a, b, c, d, GET(3) + 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, GET(11) + 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, GET(7) + 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, GET(15) + 0x6ed9eba1, 15) 

  
  a += 0x67452301; 
  b += 0xefcdab89;
  c += 0x98badcfe;
  d += 0x10325476; 
  result[0] = a;
  result[1] = b;
  result[2] = c;
  result[3] = d;
}

unsigned char set3char(int index) {
  
  if (index < 26) {
    return 'a' + index;
  } else {
    
    return '!' + (index - 26);
  }
}

__kernel void md4_crack(__global const unsigned int *target,
                        __global unsigned char *solution) {

  
  int id = get_global_id(0);
   
    unsigned char buffer[64] = {0};
    
    for (int i = 0; i < PWD_LEN; ++i) {
      if (i >= 2) {
        buffer[i] = set3char(id % 32); 
      } else {
   
        buffer[i] = (id % 26) + 'a';
      }
      id /= 26;
    }

   
    for (int i = 3; i < PWD_LEN; ++i) {
      buffer[i] = '!';
    }
 
    buffer[PWD_LEN] = 0x80;
   
    buffer[56] = PWD_LEN * 8;

    
    
    do
    {
      uint res[4];
      mybody(buffer, res);
      
      if (compare(res, target, 16) == 0) {
        
          for (int i = 0; i < PWD_LEN; ++i)  solution[i] = ((unsigned char*)buffer)[i];
      }
    } while (incr_candidate(buffer));
  }
  


