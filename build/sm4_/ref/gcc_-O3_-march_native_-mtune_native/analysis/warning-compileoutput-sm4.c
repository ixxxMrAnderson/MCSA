sm4.c: In function ‘se_be_transfer’:
sm4.c:365:24: warning: suggest parentheses around arithmetic in operand of ‘|’ [-Wparentheses]
  365 |         return (b<<24) & 0xFF000000 | (b<<8) & 0x00FF0000 | ((b>>8) & 0x0000FF00) | ((b>>24) & 0x000000FF);
      |                ~~~~~~~~^~~~~~~~~~~~
sm4.c: At top level:
sm4.c:390:1: warning: multi-line comment [-Wcomment]
  390 | //#define SM4_RNDS(B, k0, k1, k2, k3) {   \
      | ^
sm4.c:14:21: warning: ‘SM4_RND_KEY_SIZE’ defined but not used [-Wunused-const-variable=]
   14 | const static size_t SM4_RND_KEY_SIZE = 32 * 4;
      |                     ^~~~~~~~~~~~~~~~
sm4.c:13:21: warning: ‘SM4_KEY_SIZE’ defined but not used [-Wunused-const-variable=]
   13 | const static size_t SM4_KEY_SIZE = 16;
      |                     ^~~~~~~~~~~~
sm4.c:12:21: warning: ‘SM4_BLOCK_SIZE’ defined but not used [-Wunused-const-variable=]
   12 | const static size_t SM4_BLOCK_SIZE = 16;
      |                     ^~~~~~~~~~~~~~
