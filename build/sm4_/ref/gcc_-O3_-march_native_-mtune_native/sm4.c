#include <stdint.h>
#include <stddef.h>
#include <stdio.h>


typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;


const static size_t SM4_BLOCK_SIZE = 16;
const static size_t SM4_KEY_SIZE = 16;
const static size_t SM4_RND_KEY_SIZE = 32 * 4;

const u1 Sbox[256] =
        {
                0xD6, 0x90, 0xE9, 0xFE, 0xCC, 0xE1, 0x3D, 0xB7, 0x16, 0xB6, 0x14, 0xC2, 0x28, 0xFB, 0x2C, 0x05,
                0x2B, 0x67, 0x9A, 0x76, 0x2A, 0xBE, 0x04, 0xC3, 0xAA, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
                0x9C, 0x42, 0x50, 0xF4, 0x91, 0xEF, 0x98, 0x7A, 0x33, 0x54, 0x0B, 0x43, 0xED, 0xCF, 0xAC, 0x62,
                0xE4, 0xB3, 0x1C, 0xA9, 0xC9, 0x08, 0xE8, 0x95, 0x80, 0xDF, 0x94, 0xFA, 0x75, 0x8F, 0x3F, 0xA6,
                0x47, 0x07, 0xA7, 0xFC, 0xF3, 0x73, 0x17, 0xBA, 0x83, 0x59, 0x3C, 0x19, 0xE6, 0x85, 0x4F, 0xA8,
                0x68, 0x6B, 0x81, 0xB2, 0x71, 0x64, 0xDA, 0x8B, 0xF8, 0xEB, 0x0F, 0x4B, 0x70, 0x56, 0x9D, 0x35,
                0x1E, 0x24, 0x0E, 0x5E, 0x63, 0x58, 0xD1, 0xA2, 0x25, 0x22, 0x7C, 0x3B, 0x01, 0x21, 0x78, 0x87,
                0xD4, 0x00, 0x46, 0x57, 0x9F, 0xD3, 0x27, 0x52, 0x4C, 0x36, 0x02, 0xE7, 0xA0, 0xC4, 0xC8, 0x9E,
                0xEA, 0xBF, 0x8A, 0xD2, 0x40, 0xC7, 0x38, 0xB5, 0xA3, 0xF7, 0xF2, 0xCE, 0xF9, 0x61, 0x15, 0xA1,
                0xE0, 0xAE, 0x5D, 0xA4, 0x9B, 0x34, 0x1A, 0x55, 0xAD, 0x93, 0x32, 0x30, 0xF5, 0x8C, 0xB1, 0xE3,
                0x1D, 0xF6, 0xE2, 0x2E, 0x82, 0x66, 0xCA, 0x60, 0xC0, 0x29, 0x23, 0xAB, 0x0D, 0x53, 0x4E, 0x6F,
                0xD5, 0xDB, 0x37, 0x45, 0xDE, 0xFD, 0x8E, 0x2F, 0x03, 0xFF, 0x6A, 0x72, 0x6D, 0x6C, 0x5B, 0x51,
                0x8D, 0x1B, 0xAF, 0x92, 0xBB, 0xDD, 0xBC, 0x7F, 0x11, 0xD9, 0x5C, 0x41, 0x1F, 0x10, 0x5A, 0xD8,
                0x0A, 0xC1, 0x31, 0x88, 0xA5, 0xCD, 0x7B, 0xBD, 0x2D, 0x74, 0xD0, 0x12, 0xB8, 0xE5, 0xB4, 0xB0,
                0x89, 0x69, 0x97, 0x4A, 0x0C, 0x96, 0x77, 0x7E, 0x65, 0xB9, 0xF1, 0x09, 0xC5, 0x6E, 0xC6, 0x84,
                0x18, 0xF0, 0x7D, 0xEC, 0x3A, 0xDC, 0x4D, 0x20, 0x79, 0xEE, 0x5F, 0x3E, 0xD7, 0xCB, 0x39, 0x48
        };

/*
* Sbox_T[j] == L(Sbox[j]).
*/
const static u4 Sbox_T[256] =
        {
                0x5b5bd58e, 0x424292d0, 0xa7a7ea4d, 0xfbfbfd06,
                0x3333cffc, 0x8787e265, 0xf4f43dc9, 0xdedeb56b,
                0x5858164e, 0xdadab46e, 0x50501444, 0x0b0bc1ca,
                0xa0a02888, 0xefeff817, 0xb0b02c9c, 0x14140511,
                0xacac2b87, 0x9d9d66fb, 0x6a6a98f2, 0xd9d977ae,
                0xa8a82a82, 0xfafabc46, 0x10100414, 0x0f0fc0cf,
                0xaaaaa802, 0x11114554, 0x4c4c135f, 0x989826be,
                0x2525486d, 0x1a1a849e, 0x1818061e, 0x66669bfd,
                0x72729eec, 0x0909434a, 0x41415110, 0xd3d3f724,
                0x464693d5, 0xbfbfec53, 0x62629af8, 0xe9e97b92,
                0xcccc33ff, 0x51515504, 0x2c2c0b27, 0x0d0d424f,
                0xb7b7ee59, 0x3f3fccf3, 0xb2b2ae1c, 0x898963ea,
                0x9393e774, 0xceceb17f, 0x70701c6c, 0xa6a6ab0d,
                0x2727caed, 0x20200828, 0xa3a3eb48, 0x565697c1,
                0x02028280, 0x7f7fdca3, 0x525296c4, 0xebebf912,
                0xd5d574a1, 0x3e3e8db3, 0xfcfc3fc3, 0x9a9aa43e,
                0x1d1d465b, 0x1c1c071b, 0x9e9ea53b, 0xf3f3ff0c,
                0xcfcff03f, 0xcdcd72bf, 0x5c5c174b, 0xeaeab852,
                0x0e0e818f, 0x6565583d, 0xf0f03ccc, 0x6464197d,
                0x9b9be57e, 0x16168791, 0x3d3d4e73, 0xa2a2aa08,
                0xa1a169c8, 0xadad6ac7, 0x06068385, 0xcacab07a,
                0xc5c570b5, 0x919165f4, 0x6b6bd9b2, 0x2e2e89a7,
                0xe3e3fb18, 0xafafe847, 0x3c3c0f33, 0x2d2d4a67,
                0xc1c171b0, 0x5959570e, 0x76769fe9, 0xd4d435e1,
                0x78781e66, 0x909024b4, 0x38380e36, 0x79795f26,
                0x8d8d62ef, 0x61615938, 0x4747d295, 0x8a8aa02a,
                0x949425b1, 0x888822aa, 0xf1f17d8c, 0xecec3bd7,
                0x04040105, 0x848421a5, 0xe1e17998, 0x1e1e859b,
                0x5353d784, 0x00000000, 0x1919475e, 0x5d5d560b,
                0x7e7e9de3, 0x4f4fd09f, 0x9c9c27bb, 0x4949531a,
                0x31314d7c, 0xd8d836ee, 0x0808020a, 0x9f9fe47b,
                0x8282a220, 0x1313c7d4, 0x2323cbe8, 0x7a7a9ce6,
                0xababe942, 0xfefebd43, 0x2a2a88a2, 0x4b4bd19a,
                0x01014140, 0x1f1fc4db, 0xe0e038d8, 0xd6d6b761,
                0x8e8ea12f, 0xdfdff42b, 0xcbcbf13a, 0x3b3bcdf6,
                0xe7e7fa1d, 0x858560e5, 0x54541541, 0x8686a325,
                0x8383e360, 0xbabaac16, 0x75755c29, 0x9292a634,
                0x6e6e99f7, 0xd0d034e4, 0x68681a72, 0x55555401,
                0xb6b6af19, 0x4e4e91df, 0xc8c832fa, 0xc0c030f0,
                0xd7d7f621, 0x32328ebc, 0xc6c6b375, 0x8f8fe06f,
                0x74741d69, 0xdbdbf52e, 0x8b8be16a, 0xb8b82e96,
                0x0a0a808a, 0x999967fe, 0x2b2bc9e2, 0x818161e0,
                0x0303c3c0, 0xa4a4298d, 0x8c8c23af, 0xaeaea907,
                0x34340d39, 0x4d4d521f, 0x39394f76, 0xbdbd6ed3,
                0x5757d681, 0x6f6fd8b7, 0xdcdc37eb, 0x15154451,
                0x7b7bdda6, 0xf7f7fe09, 0x3a3a8cb6, 0xbcbc2f93,
                0x0c0c030f, 0xfffffc03, 0xa9a96bc2, 0xc9c973ba,
                0xb5b56cd9, 0xb1b16ddc, 0x6d6d5a37, 0x45455015,
                0x36368fb9, 0x6c6c1b77, 0xbebead13, 0x4a4a90da,
                0xeeeeb957, 0x7777dea9, 0xf2f2be4c, 0xfdfd7e83,
                0x44441155, 0x6767dabd, 0x71715d2c, 0x05054045,
                0x7c7c1f63, 0x40401050, 0x69695b32, 0x6363dbb8,
                0x28280a22, 0x0707c2c5, 0xc4c431f5, 0x22228aa8,
                0x9696a731, 0x3737cef9, 0xeded7a97, 0xf6f6bf49,
                0xb4b42d99, 0xd1d175a4, 0x4343d390, 0x4848125a,
                0xe2e2ba58, 0x9797e671, 0xd2d2b664, 0xc2c2b270,
                0x26268bad, 0xa5a568cd, 0x5e5e95cb, 0x29294b62,
                0x30300c3c, 0x5a5a94ce, 0xdddd76ab, 0xf9f97f86,
                0x959564f1, 0xe6e6bb5d, 0xc7c7f235, 0x2424092d,
                0x1717c6d1, 0xb9b96fd6, 0x1b1bc5de, 0x12128694,
                0x60601878, 0xc3c3f330, 0xf5f57c89, 0xb3b3ef5c,
                0xe8e83ad2, 0x7373dfac, 0x35354c79, 0x808020a0,
                0xe5e5789d, 0xbbbbed56, 0x7d7d5e23, 0xf8f83ec6,
                0x5f5fd48b, 0x2f2fc8e7, 0xe4e439dd, 0x21214968,
        };

const static u4 Sbox_T8[256] =
        {
                0x5bd58e5b, 0x4292d042, 0xa7ea4da7, 0xfbfd06fb,
                0x33cffc33, 0x87e26587, 0xf43dc9f4, 0xdeb56bde,
                0x58164e58, 0xdab46eda, 0x50144450, 0x0bc1ca0b,
                0xa02888a0, 0xeff817ef, 0xb02c9cb0, 0x14051114,
                0xac2b87ac, 0x9d66fb9d, 0x6a98f26a, 0xd977aed9,
                0xa82a82a8, 0xfabc46fa, 0x10041410, 0x0fc0cf0f,
                0xaaa802aa, 0x11455411, 0x4c135f4c, 0x9826be98,
                0x25486d25, 0x1a849e1a, 0x18061e18, 0x669bfd66,
                0x729eec72, 0x09434a09, 0x41511041, 0xd3f724d3,
                0x4693d546, 0xbfec53bf, 0x629af862, 0xe97b92e9,
                0xcc33ffcc, 0x51550451, 0x2c0b272c, 0x0d424f0d,
                0xb7ee59b7, 0x3fccf33f, 0xb2ae1cb2, 0x8963ea89,
                0x93e77493, 0xceb17fce, 0x701c6c70, 0xa6ab0da6,
                0x27caed27, 0x20082820, 0xa3eb48a3, 0x5697c156,
                0x02828002, 0x7fdca37f, 0x5296c452, 0xebf912eb,
                0xd574a1d5, 0x3e8db33e, 0xfc3fc3fc, 0x9aa43e9a,
                0x1d465b1d, 0x1c071b1c, 0x9ea53b9e, 0xf3ff0cf3,
                0xcff03fcf, 0xcd72bfcd, 0x5c174b5c, 0xeab852ea,
                0x0e818f0e, 0x65583d65, 0xf03cccf0, 0x64197d64,
                0x9be57e9b, 0x16879116, 0x3d4e733d, 0xa2aa08a2,
                0xa169c8a1, 0xad6ac7ad, 0x06838506, 0xcab07aca,
                0xc570b5c5, 0x9165f491, 0x6bd9b26b, 0x2e89a72e,
                0xe3fb18e3, 0xafe847af, 0x3c0f333c, 0x2d4a672d,
                0xc171b0c1, 0x59570e59, 0x769fe976, 0xd435e1d4,
                0x781e6678, 0x9024b490, 0x380e3638, 0x795f2679,
                0x8d62ef8d, 0x61593861, 0x47d29547, 0x8aa02a8a,
                0x9425b194, 0x8822aa88, 0xf17d8cf1, 0xec3bd7ec,
                0x04010504, 0x8421a584, 0xe17998e1, 0x1e859b1e,
                0x53d78453, 0x00000000, 0x19475e19, 0x5d560b5d,
                0x7e9de37e, 0x4fd09f4f, 0x9c27bb9c, 0x49531a49,
                0x314d7c31, 0xd836eed8, 0x08020a08, 0x9fe47b9f,
                0x82a22082, 0x13c7d413, 0x23cbe823, 0x7a9ce67a,
                0xabe942ab, 0xfebd43fe, 0x2a88a22a, 0x4bd19a4b,
                0x01414001, 0x1fc4db1f, 0xe038d8e0, 0xd6b761d6,
                0x8ea12f8e, 0xdff42bdf, 0xcbf13acb, 0x3bcdf63b,
                0xe7fa1de7, 0x8560e585, 0x54154154, 0x86a32586,
                0x83e36083, 0xbaac16ba, 0x755c2975, 0x92a63492,
                0x6e99f76e, 0xd034e4d0, 0x681a7268, 0x55540155,
                0xb6af19b6, 0x4e91df4e, 0xc832fac8, 0xc030f0c0,
                0xd7f621d7, 0x328ebc32, 0xc6b375c6, 0x8fe06f8f,
                0x741d6974, 0xdbf52edb, 0x8be16a8b, 0xb82e96b8,
                0x0a808a0a, 0x9967fe99, 0x2bc9e22b, 0x8161e081,
                0x03c3c003, 0xa4298da4, 0x8c23af8c, 0xaea907ae,
                0x340d3934, 0x4d521f4d, 0x394f7639, 0xbd6ed3bd,
                0x57d68157, 0x6fd8b76f, 0xdc37ebdc, 0x15445115,
                0x7bdda67b, 0xf7fe09f7, 0x3a8cb63a, 0xbc2f93bc,
                0x0c030f0c, 0xfffc03ff, 0xa96bc2a9, 0xc973bac9,
                0xb56cd9b5, 0xb16ddcb1, 0x6d5a376d, 0x45501545,
                0x368fb936, 0x6c1b776c, 0xbead13be, 0x4a90da4a,
                0xeeb957ee, 0x77dea977, 0xf2be4cf2, 0xfd7e83fd,
                0x44115544, 0x67dabd67, 0x715d2c71, 0x05404505,
                0x7c1f637c, 0x40105040, 0x695b3269, 0x63dbb863,
                0x280a2228, 0x07c2c507, 0xc431f5c4, 0x228aa822,
                0x96a73196, 0x37cef937, 0xed7a97ed, 0xf6bf49f6,
                0xb42d99b4, 0xd175a4d1, 0x43d39043, 0x48125a48,
                0xe2ba58e2, 0x97e67197, 0xd2b664d2, 0xc2b270c2,
                0x268bad26, 0xa568cda5, 0x5e95cb5e, 0x294b6229,
                0x300c3c30, 0x5a94ce5a, 0xdd76abdd, 0xf97f86f9,
                0x9564f195, 0xe6bb5de6, 0xc7f235c7, 0x24092d24,
                0x17c6d117, 0xb96fd6b9, 0x1bc5de1b, 0x12869412,
                0x60187860, 0xc3f330c3, 0xf57c89f5, 0xb3ef5cb3,
                0xe83ad2e8, 0x73dfac73, 0x354c7935, 0x8020a080,
                0xe5789de5, 0xbbed56bb, 0x7d5e237d, 0xf83ec6f8,
                0x5fd48b5f, 0x2fc8e72f, 0xe439dde4, 0x21496821,
        };

const static u4 Sbox_T16[256] =
        {
                0xd58e5b5b, 0x92d04242, 0xea4da7a7, 0xfd06fbfb,
                0xcffc3333, 0xe2658787, 0x3dc9f4f4, 0xb56bdede,
                0x164e5858, 0xb46edada, 0x14445050, 0xc1ca0b0b,
                0x2888a0a0, 0xf817efef, 0x2c9cb0b0, 0x05111414,
                0x2b87acac, 0x66fb9d9d, 0x98f26a6a, 0x77aed9d9,
                0x2a82a8a8, 0xbc46fafa, 0x04141010, 0xc0cf0f0f,
                0xa802aaaa, 0x45541111, 0x135f4c4c, 0x26be9898,
                0x486d2525, 0x849e1a1a, 0x061e1818, 0x9bfd6666,
                0x9eec7272, 0x434a0909, 0x51104141, 0xf724d3d3,
                0x93d54646, 0xec53bfbf, 0x9af86262, 0x7b92e9e9,
                0x33ffcccc, 0x55045151, 0x0b272c2c, 0x424f0d0d,
                0xee59b7b7, 0xccf33f3f, 0xae1cb2b2, 0x63ea8989,
                0xe7749393, 0xb17fcece, 0x1c6c7070, 0xab0da6a6,
                0xcaed2727, 0x08282020, 0xeb48a3a3, 0x97c15656,
                0x82800202, 0xdca37f7f, 0x96c45252, 0xf912ebeb,
                0x74a1d5d5, 0x8db33e3e, 0x3fc3fcfc, 0xa43e9a9a,
                0x465b1d1d, 0x071b1c1c, 0xa53b9e9e, 0xff0cf3f3,
                0xf03fcfcf, 0x72bfcdcd, 0x174b5c5c, 0xb852eaea,
                0x818f0e0e, 0x583d6565, 0x3cccf0f0, 0x197d6464,
                0xe57e9b9b, 0x87911616, 0x4e733d3d, 0xaa08a2a2,
                0x69c8a1a1, 0x6ac7adad, 0x83850606, 0xb07acaca,
                0x70b5c5c5, 0x65f49191, 0xd9b26b6b, 0x89a72e2e,
                0xfb18e3e3, 0xe847afaf, 0x0f333c3c, 0x4a672d2d,
                0x71b0c1c1, 0x570e5959, 0x9fe97676, 0x35e1d4d4,
                0x1e667878, 0x24b49090, 0x0e363838, 0x5f267979,
                0x62ef8d8d, 0x59386161, 0xd2954747, 0xa02a8a8a,
                0x25b19494, 0x22aa8888, 0x7d8cf1f1, 0x3bd7ecec,
                0x01050404, 0x21a58484, 0x7998e1e1, 0x859b1e1e,
                0xd7845353, 0x00000000, 0x475e1919, 0x560b5d5d,
                0x9de37e7e, 0xd09f4f4f, 0x27bb9c9c, 0x531a4949,
                0x4d7c3131, 0x36eed8d8, 0x020a0808, 0xe47b9f9f,
                0xa2208282, 0xc7d41313, 0xcbe82323, 0x9ce67a7a,
                0xe942abab, 0xbd43fefe, 0x88a22a2a, 0xd19a4b4b,
                0x41400101, 0xc4db1f1f, 0x38d8e0e0, 0xb761d6d6,
                0xa12f8e8e, 0xf42bdfdf, 0xf13acbcb, 0xcdf63b3b,
                0xfa1de7e7, 0x60e58585, 0x15415454, 0xa3258686,
                0xe3608383, 0xac16baba, 0x5c297575, 0xa6349292,
                0x99f76e6e, 0x34e4d0d0, 0x1a726868, 0x54015555,
                0xaf19b6b6, 0x91df4e4e, 0x32fac8c8, 0x30f0c0c0,
                0xf621d7d7, 0x8ebc3232, 0xb375c6c6, 0xe06f8f8f,
                0x1d697474, 0xf52edbdb, 0xe16a8b8b, 0x2e96b8b8,
                0x808a0a0a, 0x67fe9999, 0xc9e22b2b, 0x61e08181,
                0xc3c00303, 0x298da4a4, 0x23af8c8c, 0xa907aeae,
                0x0d393434, 0x521f4d4d, 0x4f763939, 0x6ed3bdbd,
                0xd6815757, 0xd8b76f6f, 0x37ebdcdc, 0x44511515,
                0xdda67b7b, 0xfe09f7f7, 0x8cb63a3a, 0x2f93bcbc,
                0x030f0c0c, 0xfc03ffff, 0x6bc2a9a9, 0x73bac9c9,
                0x6cd9b5b5, 0x6ddcb1b1, 0x5a376d6d, 0x50154545,
                0x8fb93636, 0x1b776c6c, 0xad13bebe, 0x90da4a4a,
                0xb957eeee, 0xdea97777, 0xbe4cf2f2, 0x7e83fdfd,
                0x11554444, 0xdabd6767, 0x5d2c7171, 0x40450505,
                0x1f637c7c, 0x10504040, 0x5b326969, 0xdbb86363,
                0x0a222828, 0xc2c50707, 0x31f5c4c4, 0x8aa82222,
                0xa7319696, 0xcef93737, 0x7a97eded, 0xbf49f6f6,
                0x2d99b4b4, 0x75a4d1d1, 0xd3904343, 0x125a4848,
                0xba58e2e2, 0xe6719797, 0xb664d2d2, 0xb270c2c2,
                0x8bad2626, 0x68cda5a5, 0x95cb5e5e, 0x4b622929,
                0x0c3c3030, 0x94ce5a5a, 0x76abdddd, 0x7f86f9f9,
                0x64f19595, 0xbb5de6e6, 0xf235c7c7, 0x092d2424,
                0xc6d11717, 0x6fd6b9b9, 0xc5de1b1b, 0x86941212,
                0x18786060, 0xf330c3c3, 0x7c89f5f5, 0xef5cb3b3,
                0x3ad2e8e8, 0xdfac7373, 0x4c793535, 0x20a08080,
                0x789de5e5, 0xed56bbbb, 0x5e237d7d, 0x3ec6f8f8,
                0xd48b5f5f, 0xc8e72f2f, 0x39dde4e4, 0x49682121,
        };

const static u4 Sbox_T24[256] =
        {
                0x8e5b5bd5, 0xd0424292, 0x4da7a7ea, 0x06fbfbfd,
                0xfc3333cf, 0x658787e2, 0xc9f4f43d, 0x6bdedeb5,
                0x4e585816, 0x6edadab4, 0x44505014, 0xca0b0bc1,
                0x88a0a028, 0x17efeff8, 0x9cb0b02c, 0x11141405,
                0x87acac2b, 0xfb9d9d66, 0xf26a6a98, 0xaed9d977,
                0x82a8a82a, 0x46fafabc, 0x14101004, 0xcf0f0fc0,
                0x02aaaaa8, 0x54111145, 0x5f4c4c13, 0xbe989826,
                0x6d252548, 0x9e1a1a84, 0x1e181806, 0xfd66669b,
                0xec72729e, 0x4a090943, 0x10414151, 0x24d3d3f7,
                0xd5464693, 0x53bfbfec, 0xf862629a, 0x92e9e97b,
                0xffcccc33, 0x04515155, 0x272c2c0b, 0x4f0d0d42,
                0x59b7b7ee, 0xf33f3fcc, 0x1cb2b2ae, 0xea898963,
                0x749393e7, 0x7fceceb1, 0x6c70701c, 0x0da6a6ab,
                0xed2727ca, 0x28202008, 0x48a3a3eb, 0xc1565697,
                0x80020282, 0xa37f7fdc, 0xc4525296, 0x12ebebf9,
                0xa1d5d574, 0xb33e3e8d, 0xc3fcfc3f, 0x3e9a9aa4,
                0x5b1d1d46, 0x1b1c1c07, 0x3b9e9ea5, 0x0cf3f3ff,
                0x3fcfcff0, 0xbfcdcd72, 0x4b5c5c17, 0x52eaeab8,
                0x8f0e0e81, 0x3d656558, 0xccf0f03c, 0x7d646419,
                0x7e9b9be5, 0x91161687, 0x733d3d4e, 0x08a2a2aa,
                0xc8a1a169, 0xc7adad6a, 0x85060683, 0x7acacab0,
                0xb5c5c570, 0xf4919165, 0xb26b6bd9, 0xa72e2e89,
                0x18e3e3fb, 0x47afafe8, 0x333c3c0f, 0x672d2d4a,
                0xb0c1c171, 0x0e595957, 0xe976769f, 0xe1d4d435,
                0x6678781e, 0xb4909024, 0x3638380e, 0x2679795f,
                0xef8d8d62, 0x38616159, 0x954747d2, 0x2a8a8aa0,
                0xb1949425, 0xaa888822, 0x8cf1f17d, 0xd7ecec3b,
                0x05040401, 0xa5848421, 0x98e1e179, 0x9b1e1e85,
                0x845353d7, 0x00000000, 0x5e191947, 0x0b5d5d56,
                0xe37e7e9d, 0x9f4f4fd0, 0xbb9c9c27, 0x1a494953,
                0x7c31314d, 0xeed8d836, 0x0a080802, 0x7b9f9fe4,
                0x208282a2, 0xd41313c7, 0xe82323cb, 0xe67a7a9c,
                0x42ababe9, 0x43fefebd, 0xa22a2a88, 0x9a4b4bd1,
                0x40010141, 0xdb1f1fc4, 0xd8e0e038, 0x61d6d6b7,
                0x2f8e8ea1, 0x2bdfdff4, 0x3acbcbf1, 0xf63b3bcd,
                0x1de7e7fa, 0xe5858560, 0x41545415, 0x258686a3,
                0x608383e3, 0x16babaac, 0x2975755c, 0x349292a6,
                0xf76e6e99, 0xe4d0d034, 0x7268681a, 0x01555554,
                0x19b6b6af, 0xdf4e4e91, 0xfac8c832, 0xf0c0c030,
                0x21d7d7f6, 0xbc32328e, 0x75c6c6b3, 0x6f8f8fe0,
                0x6974741d, 0x2edbdbf5, 0x6a8b8be1, 0x96b8b82e,
                0x8a0a0a80, 0xfe999967, 0xe22b2bc9, 0xe0818161,
                0xc00303c3, 0x8da4a429, 0xaf8c8c23, 0x07aeaea9,
                0x3934340d, 0x1f4d4d52, 0x7639394f, 0xd3bdbd6e,
                0x815757d6, 0xb76f6fd8, 0xebdcdc37, 0x51151544,
                0xa67b7bdd, 0x09f7f7fe, 0xb63a3a8c, 0x93bcbc2f,
                0x0f0c0c03, 0x03fffffc, 0xc2a9a96b, 0xbac9c973,
                0xd9b5b56c, 0xdcb1b16d, 0x376d6d5a, 0x15454550,
                0xb936368f, 0x776c6c1b, 0x13bebead, 0xda4a4a90,
                0x57eeeeb9, 0xa97777de, 0x4cf2f2be, 0x83fdfd7e,
                0x55444411, 0xbd6767da, 0x2c71715d, 0x45050540,
                0x637c7c1f, 0x50404010, 0x3269695b, 0xb86363db,
                0x2228280a, 0xc50707c2, 0xf5c4c431, 0xa822228a,
                0x319696a7, 0xf93737ce, 0x97eded7a, 0x49f6f6bf,
                0x99b4b42d, 0xa4d1d175, 0x904343d3, 0x5a484812,
                0x58e2e2ba, 0x719797e6, 0x64d2d2b6, 0x70c2c2b2,
                0xad26268b, 0xcda5a568, 0xcb5e5e95, 0x6229294b,
                0x3c30300c, 0xce5a5a94, 0xabdddd76, 0x86f9f97f,
                0xf1959564, 0x5de6e6bb, 0x35c7c7f2, 0x2d242409,
                0xd11717c6, 0xd6b9b96f, 0xde1b1bc5, 0x94121286,
                0x78606018, 0x30c3c3f3, 0x89f5f57c, 0x5cb3b3ef,
                0xd2e8e83a, 0xac7373df, 0x7935354c, 0xa0808020,
                0x9de5e578, 0x56bbbbed, 0x237d7d5e, 0xc6f8f83e,
                0x8b5f5fd4, 0xe72f2fc8, 0xdde4e439, 0x68212149,
        };


/**
* Bit rotation left by a compile-time constant amount
* @param input the input word
* @return input rotated left by ROT bits
*/
//template<size_t ROT, typename T>
//inline T rotl(T input)
//{
//    static_assert(ROT > 0 && ROT < 8*sizeof(T), "Invalid rotation constant");
//    return static_cast<T>((input << ROT) | (input >> (8*sizeof(T) - ROT)));
//}

/**
* Bit rotation right by a compile-time constant amount
* @param input the input word
* @return input rotated right by ROT bits
*/
//template<size_t ROT, typename T>
//inline T rotr(T input)
//{
//    static_assert(ROT > 0 && ROT < 8*sizeof(T), "Invalid rotation constant");
//    return static_cast<T>((input >> ROT) | (input << (8*sizeof(T) - ROT)));
//}

/**
* Byte extraction
* @param byte_num which byte to extract, 0 == highest byte
* @param input the value to extract from
* @return byte byte_num of input
*/
/*inline*/ u1 get_byte(size_t byte_num, u4 input)
{
    return (u4)(input >> ((sizeof(u4)-1-(byte_num&(sizeof(u4)-1))) << 3));
}

/**
* Make a uint32_t from four bytes
* @param i0 the first byte
* @param i1 the second byte
* @param i2 the third byte
* @param i3 the fourth byte
* @return i0 || i1 || i2 || i3
*/
//inline uint32_t make_uint32(uint8_t i0, uint8_t i1, uint8_t i2, uint8_t i3)
//{
//    return ((static_cast<uint32_t>(i0) << 24) |
//    (static_cast<uint32_t>(i1) << 16) |
//    (static_cast<uint32_t>(i2) <<  8) |
//    (static_cast<uint32_t>(i3)));
//}

u4 se_be_transfer(u4 b){

        return (b<<24) & 0xFF000000 | (b<<8) & 0x00FF0000 | ((b>>8) & 0x0000FF00) | ((b>>24) & 0x000000FF);
}

// u4 SM4_T(u4 b)
// {
//     return Sbox_T24[get_byte(0,b)] ^ Sbox_T16[get_byte(1,b)] ^ Sbox_T8[get_byte(2,b)] ^ Sbox_T[get_byte(3,b)];
// }

u4 SM4_T(u4 b)
{       
    b = se_be_transfer(b); //suppose input b is be. we need to transfer it to se in our sm4.
    //the result is se, we need to translate it to be again.
    return se_be_transfer(Sbox_T24[get_byte(0,b)] ^ Sbox_T16[get_byte(1,b)] ^ Sbox_T8[get_byte(2,b)] ^ Sbox_T[get_byte(3,b)]);
}


// Variant of T for key schedule
//inline u4 SM4_Tp(u4 b)
//{
//    const u4 t = make_uint32(Sbox[get_byte(0,b)], Sbox[get_byte(1,b)], Sbox[get_byte(2,b)], Sbox[get_byte(3,b)]);
//
//    // L' linear transform
//    return t ^ rotl<13>(t) ^ rotl<23>(t);
//}
//
//#define SM4_RNDS(B, k0, k1, k2, k3) {   \
//  B[0] ^= F(B[1] ^ B[2] ^ B[3] ^ rkey[k0]); \
//  B[1] ^= F(B[0] ^ B[2] ^ B[3] ^ rkey[k1]); \
//  B[2] ^= F(B[0] ^ B[1] ^ B[3] ^ rkey[k2]); \
//  B[3] ^= F(B[0] ^ B[1] ^ B[2] ^ rkey[k3]); \
//}
//
///*
//* SM4 Encryption
//*/
// void SM4_enc_block(u4 block[SM4_BLOCK_SIZE / sizeof u4], const u4 rkey[SM4_RND_KEY_SIZE / sizeof u4])
// {
//    SM4_RNDS(block, 0,  1,  2,  3);
//    SM4_RNDS(block, 4,  5,  6,  7);
//    SM4_RNDS(block, 8,  9, 10, 11);
//    SM4_RNDS(block, 12, 13, 14, 15);
//    SM4_RNDS(block, 16, 17, 18, 19);
//    SM4_RNDS(block, 20, 21, 22, 23);
//    SM4_RNDS(block, 24, 25, 26, 27);
//    SM4_RNDS(block, 28, 29, 30, 31);

//    static u4 interim;
//    interim = block[0]; block[0] = block[3]; block[3] = interim;
//    interim = block[1]; block[1] = block[2]; block[2] = interim;
// }
//
///*
//* SM4 Decryption
//*/
//void SM4_dec_block(u4 block[SM4_BLOCK_SIZE], const u4 rkey[SM4_RND_KEY_SIZE / sizeof u4])
//{
//    SM4_RNDS(block, 31, 30, 29, 28);
//    SM4_RNDS(block, 27, 26, 25, 24);
//    SM4_RNDS(block, 23, 22, 21, 20);
//    SM4_RNDS(block, 19, 18, 17, 16);
//    SM4_RNDS(block, 15, 14, 13, 12);
//    SM4_RNDS(block, 11, 10,  9,  8);
//    SM4_RNDS(block,  7,  6,  5,  4);
//    SM4_RNDS(block,  3,  2,  1,  0);
//
//    static u4 interim;
//    interim = block[0]; block[0] = block[3]; block[3] = interim;
//    interim = block[1]; block[1] = block[2]; block[2] = interim;
//}
//
//#undef SM4_RNDS



// static void outputChar(uint8_t* text, size_t len)
// {
//         for (size_t i = 0; i < len; ++i)
//         {
//                 printf("0x%02x, ", text[i]);
//                 if (i % 16 == 15)
//                         puts("");
//         }
//         puts("");
// }
// //#include <time.h>
// int main()
// {
//         // plain: 01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
//         // key:   01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
//         // cipher: 68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46      
//         // u1 key[SM4_KEY_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
//         // u1 p[SM4_BLOCK_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
//         // u1 c[SM4_BLOCK_SIZE];

//         // u4 rkey[SM4_RND_KEY_SIZE / sizeof u4];
//         // SM4_key_schedule( key, rkey );
        
//         // u4 block[4];
//         // memcpy(block, p, 16);
                
//         // SM4_enc_block( block, rkey );
//         // memcpy(c, block, 16);

//         // // should be: 0x68, 0x1e, 0xdf, 0x34, 0xd2, 0x06, 0x96, 0x5e, 0x86, 0xb3, 0xe9, 0x4f, 0x53, 0x6e, 0x42, 0x46,
//         // outputChar(c, sizeof c);

//         // memcpy(block, c, 16);
//         // SM4_dec_block( block, rkey );
//         // memcpy(p, block, 16);
//         // outputChar(p, sizeof p);

//         // // return 0;
        
//         // clock_t t = clock();
        
//         // forloop (i, 0, 10000000)
//         // {
//         //         SM4_enc_block( block, rkey );
//         // }

//         // printf("time: %d ms\n", clock() - t );

//         u4 b = 0x12345678;

//         u4 ans = SM4_T(b);

//         outputChar((uint8_t*)&ans, sizeof(ans));


// }

