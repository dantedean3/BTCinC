#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROTR(x,n) ((x >> n) | (x << (32 - n)))
#define SHR(x,n)  (x >> n)

#define CH(x,y,z)  ((x & y) ^ (~x & z))
#define MAJ(x,y,z) ((x & y) ^ (x & z) ^ (y & z))
#define BSIG0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define BSIG1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))
#define SSIG0(x) (ROTR(x,7) ^ ROTR(x,18) ^ SHR(x,3))
#define SSIG1(x) (ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10))

static const uint32_t k[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static const uint32_t h_init[8] = {
  0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
  0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
};

static uint32_t to_uint32_be(const unsigned char *p) {
  return ((uint32_t)p[0] << 24) |
         ((uint32_t)p[1] << 16) |
         ((uint32_t)p[2] << 8)  |
         ((uint32_t)p[3]);
}

static void from_uint32_be(uint32_t val, unsigned char *out) {
  out[0] = (unsigned char)((val >> 24) & 0xff);
  out[1] = (unsigned char)((val >> 16) & 0xff);
  out[2] = (unsigned char)((val >> 8) & 0xff);
  out[3] = (unsigned char)(val & 0xff);
}

void sha256(const unsigned char *msg, size_t len, unsigned char *digest) {
  uint32_t h[8];
  unsigned char *padded;
  size_t new_len, offset;
  uint64_t bit_len;
  int i;

  memcpy(h, h_init, sizeof(h_init));

  new_len = len + 1;
  while ((new_len % 64) != 56) new_len++;
  padded = (unsigned char *)calloc(new_len + 8, 1);
  if (!padded) return;

  memcpy(padded, msg, len);
  padded[len] = 0x80;

  bit_len = (uint64_t)len * 8;
  for (i = 0; i < 8; i++) {
    padded[new_len + i] = (unsigned char)((bit_len >> (56 - 8 * i)) & 0xff);
  }

  for (offset = 0; offset < new_len + 8; offset += 64) {
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, hh;
    uint32_t temp1, temp2;
    int t;

    for (t = 0; t < 16; t++) {
      w[t] = to_uint32_be(padded + offset + t * 4);
    }
    for (t = 16; t < 64; t++) {
      w[t] = SSIG1(w[t - 2]) + w[t - 7] + SSIG0(w[t - 15]) + w[t - 16];
    }

    a = h[0]; b = h[1]; c = h[2]; d = h[3];
    e = h[4]; f = h[5]; g = h[6]; hh = h[7];

    for (t = 0; t < 64; t++) {
      temp1 = hh + BSIG1(e) + CH(e,f,g) + k[t] + w[t];
      temp2 = BSIG0(a) + MAJ(a,b,c);
      hh = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
    }

    h[0] += a; h[1] += b; h[2] += c; h[3] += d;
    h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
  }

  for (i = 0; i < 8; i++) {
    from_uint32_be(h[i], digest + i * 4);
  }

  free(padded);
}

int main(int argc, char *argv[]) {
  FILE *fp;
  unsigned char *msg;
  long len;
  unsigned char digest[32];
  int i;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  msg = (unsigned char *)malloc(len);
  if (!msg) {
    fclose(fp);
    fprintf(stderr, "Memory error\n");
    return 1;
  }

  if (fread(msg, 1, len, fp) != (size_t)len) {
    fclose(fp);
    free(msg);
    fprintf(stderr, "Read error\n");
    return 1;
  }
  fclose(fp);

  sha256(msg, len, digest);
  free(msg);

  for (i = 0; i < 32; i++) {
    printf("%02x", digest[i]);
  }
  printf("  %s\n", argv[1]);
  return 0;
}
