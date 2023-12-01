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

int my_memcmp(const unsigned char *s1, const unsigned char *s2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            return (s1[i] - s2[i]);
        }
    }
    return 0;
}

int incr_candidate(char *ptr)
{
  size_t pos = PWD_LEN - 1; // -3 car ça décrémente et puis on décrémente pas jusqu'au bout car on connait dékàà les 3 premiers acrac qui sont fixées
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
  STEP(F, d, a, b, c, SET(1), 7) // SET(1) = 1er bloc de 4 octets
  // On peut tout retirer sauf les 2 premiers et le 14eme
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
  STEP(H2, d, a, b, c, GET(9) + 0x6ed9eba1, 9) // De là à 
  STEP(H, c, d, a, b, GET(5) + 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, GET(13) + 0x6ed9eba1, 15)
  STEP(H, a, b, c, d, GET(3) + 0x6ed9eba1, 3)
  STEP(H2, d, a, b, c, GET(11) + 0x6ed9eba1, 9)
  STEP(H, c, d, a, b, GET(7) + 0x6ed9eba1, 11)
  STEP(H2, b, c, d, a, GET(15) + 0x6ed9eba1, 15) // là on peut retirer (15 veut dire qu'on fini par une rotation de 15) --> y'a que b qui est modifié ici

  // On doit implémenter le step inverse avant la boucle 
  a += 0x67452301; // Faire une soustraction plutôt 
  b += 0xefcdab89;
  c += 0x98badcfe;
  d += 0x10325476; 
  result[0] = a;
  result[1] = b;
  result[2] = c;
  result[3] = d;
}

unsigned char trouver_mon_caractere(int index) {
  // Si l'index est dans la plage des lettres minuscules
  if (index < 26) {
    return 'a' + index;
  } else {
    // Si l'index dépasse 26, renvoyer un caractère spécial basé sur la valeur ASCII
    return '!' + (index - 26);
  }
}

__kernel void md4_crack(__global const unsigned int *target,
                        __global unsigned char *solution) {

  // Get the index of the current element to be processed
  int id = get_global_id(0);
    //printf("%d\t", id);
    unsigned char buffer[64] = {0};
    // Initialisation du buffer
    for (int i = 0; i < PWD_LEN; ++i) {
      if (i >= 2) {
        buffer[i] = trouver_mon_caractere(id % 32); // C'est que pour le 32 les carctères spéciaux
      } else {
        // Que du lowercase
        buffer[i] = (id % 26) + 'a';
      }
      id /= 26;
    }

    // Candidate à reconstruire
    for (int i = 3; i < PWD_LEN; ++i) {
      buffer[i] = '!';
    }
    // Mettre le bit 0x80 à la position PWD_LEN
    buffer[PWD_LEN] = 0x80;
    // Mettre la longueur du mot de passe à la position 56
    buffer[56] = PWD_LEN * 8;
    //  contenu du buffer (en hexadécimal)
    
    do
    {
      uint res[16];
      mybody(buffer, res);
      // my_memcmp pour remplacer memcmp
      if (my_memcmp(res, target, sizeof(res)) == 0) {
          printf("found: %s\n", buffer);
      }

    } while (incr_candidate(buffer));
  }
  

  // À ce stade, le buffer contient une combinaison de mot de passe basée sur l'ID du thread
  // Vous devrez implémenter une boucle pour générer différentes combinaisons de mots de passe
  // et vérifier si le hachage correspond à la cible. Si oui, copiez-le dans la solution.



// Il récupère ci dessus le thread courant et on doit faire une boucle qui va cracker des mdp ? Comme le md d'hier
// Target = hash à casser --> tablea de 4 int *
// Dans solution on écrit la sol° et donc le bon hash
// On doit faire attention au indice du thread car y'en a 26 * 26 * 32
// commande pour exécuter le main: ./target/gpu gpu-opencl/vector_md4_kernel.cl 804e7f1c2586e50b49ac65db5b645131