// acquired from http://www.cr0.net:8040/code/crypto/
// original code header:
/*
 *  FIPS-180-2 compliant SHA-256 implementation
 *
 *  Copyright (C) 2001-2003  Christophe Devine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef KCHECKSUM_H
#define KCHECKSUM_H

#include "KFile.h"

#include <fstream>
#include <iomanip>

enum KChecksumType
{
    MD5, SHA1, SHA256
};

class KChecksum
{
public:

    KChecksum(const KChecksumType type)
    {
        m_type = type;
        switch(m_type)
        {
            case MD5:
                m_state.MD5.start();
                break;
            case SHA1:
                m_state.SHA1.start();
                break;
            case SHA256:
                m_state.SHA256.start();
                break;
            default:
                m_state.SHA256.start();
                break;
        }
    }

    virtual ~KChecksum() { }

    /**
     * Add data to the checksum.
     */
    bool add(const char *Data, const int Length)
    {
        unsigned char *uData = (unsigned char *) Data;
        return add(uData, Length);
    }

    /**
     * Add data to the checksum.
     */
    bool add(const unsigned char *Data, const int Length)
    {
        unsigned char *uData = (unsigned char *) Data;
        return add(uData, Length);
    }

    /**
     * Add data to the checksum.
     */
    bool add(char *Data, const int Length)
    {
        unsigned char *uData = (unsigned char *) Data;
        return add(uData, Length);
    }

    /**
     * Add data to the checksum.
     */
    bool add(unsigned char *Data, const int Length)
    {
        switch(m_type)
        {
            case MD5:
                m_state.MD5.update(Data, Length);
                break;
            case SHA1:
                m_state.SHA1.update(Data, Length);
                break;
            case SHA256:
                m_state.SHA256.update(Data, Length);
                break;
            default:
                m_state.SHA256.update(Data, Length);
                break;
        }

        return true;
    }

    /**
     * Add file to the checksum.
     * @param fileName the file to add.
     */
    bool addFile(std::string fileName)
    {
        try
        {
            if(!KFile::exists(fileName))
            {
                return false;
            }
            if(!KFile::isFile(fileName))
            {
                return false;
            }

            std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
            if(!file.is_open())
            {
                return false;
            }
            std::ifstream::pos_type fSize = file.tellg();
            char buf[1024];
            int readSize = 1024;
            while(!file.eof())
            {
                file.read(buf, 1024);
                int rd = (int) file.gcount();
                add(buf, rd);
                fSize -= 1024;
                if(fSize < readSize)
                {
                    readSize = fSize;
                }
            }
            file.close();
        }
        catch(std::exception &e)
        {
            return false;
        }
        return true;
    }

    /**
     * retrieve the checksum hash string.
     * @return The checksum string.
     */
    std::string getHashString()
    {
        unsigned char sum[32];
        int i;
        std::ostringstream ss;
        State state(m_state);
        switch(m_type)
        {
            case MD5:
                state.MD5.finish(sum);

                ss << std::hex << std::setfill('0') << std::uppercase;
                for(i = 0; i < 16; i++)
                {
                    ss << std::setw(2) << static_cast<int> (sum[i]);
                }
                break;
            case SHA1:
                state.SHA1.finish(sum);

                ss << std::hex << std::setfill('0') << std::uppercase;
                for(i = 0; i < 20; i++)
                {
                    ss << std::setw(2) << static_cast<int> (sum[i]);
                }
                break;
            case SHA256:
                state.SHA256.finish(sum);

                ss << std::hex << std::setfill('0') << std::uppercase;
                for(i = 0; i < 32; i++)
                {
                    ss << std::setw(2) << static_cast<int> (sum[i]);
                }
                break;
            default:
                state.SHA256.finish(sum);

                ss << std::hex << std::setfill('0') << std::uppercase;
                for(i = 0; i < 32; i++)
                {
                    ss << std::setw(2) << static_cast<int> (sum[i]);
                }
                break;
        }

        return ss.str();
    }

    /**
     * The type of checksum operation being performed.
     */
    KChecksumType type() const
    {
        return m_type;
    }

protected:
    KChecksumType m_type;

#define GET_UINT32(n,b,i) { \
      (n) = ((unsigned long int)(b)[(i)] << 24) \
      | ((unsigned long int)(b)[(i)+1] << 16) \
      | ((unsigned long int)(b)[(i)+2] << 8) \
      | ((unsigned long int)(b)[(i)+3]); \
        }

#define PUT_UINT32(n,b,i) { \
      (b)[(i)] = (unsigned char)((n) >> 24); \
      (b)[(i)+1] = (unsigned char)((n) >> 16); \
      (b)[(i)+2] = (unsigned char)((n) >> 8); \
      (b)[(i)+3] = (unsigned char)((n)); \
      }

    //---------------------------------------
    // SHA256

    typedef struct
    {
    private:
        unsigned long int m_total[2];
        unsigned long int m_state[8];
        unsigned char m_buffer[64];

    public:

        void start()
        {
            m_total[0] = 0;
            m_total[1] = 0;

            m_state[0] = 0x6A09E667;
            m_state[1] = 0xBB67AE85;
            m_state[2] = 0x3C6EF372;
            m_state[3] = 0xA54FF53A;
            m_state[4] = 0x510E527F;
            m_state[5] = 0x9B05688C;
            m_state[6] = 0x1F83D9AB;
            m_state[7] = 0x5BE0CD19;
        }

        void process(const unsigned char data[64])
        {
            unsigned long int temp1, temp2, W[64];
            unsigned long int A, B, C, D, E, F, G, H;

            GET_UINT32(W[0], data, 0);
            GET_UINT32(W[1], data, 4);
            GET_UINT32(W[2], data, 8);
            GET_UINT32(W[3], data, 12);
            GET_UINT32(W[4], data, 16);
            GET_UINT32(W[5], data, 20);
            GET_UINT32(W[6], data, 24);
            GET_UINT32(W[7], data, 28);
            GET_UINT32(W[8], data, 32);
            GET_UINT32(W[9], data, 36);
            GET_UINT32(W[10], data, 40);
            GET_UINT32(W[11], data, 44);
            GET_UINT32(W[12], data, 48);
            GET_UINT32(W[13], data, 52);
            GET_UINT32(W[14], data, 56);
            GET_UINT32(W[15], data, 60);

#define  SHR(x,n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))

#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3))
#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))

#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))

#define R256(t) ( W[t] = S1(W[t - 2]) + W[t - 7] + S0(W[t - 15]) + W[t - 16] )

#define P256(a,b,c,d,e,f,g,h,x,K) { \
  temp1 = h + S3(e) + F1(e, f, g) + K + x;      \
  temp2 = S2(a) + F0(a, b, c);                  \
  d += temp1; h = temp1 + temp2;              \
    }

            A = m_state[0];
            B = m_state[1];
            C = m_state[2];
            D = m_state[3];
            E = m_state[4];
            F = m_state[5];
            G = m_state[6];
            H = m_state[7];

            P256(A, B, C, D, E, F, G, H, W[0], 0x428A2F98);
            P256(H, A, B, C, D, E, F, G, W[1], 0x71374491);
            P256(G, H, A, B, C, D, E, F, W[2], 0xB5C0FBCF);
            P256(F, G, H, A, B, C, D, E, W[3], 0xE9B5DBA5);
            P256(E, F, G, H, A, B, C, D, W[4], 0x3956C25B);
            P256(D, E, F, G, H, A, B, C, W[5], 0x59F111F1);
            P256(C, D, E, F, G, H, A, B, W[6], 0x923F82A4);
            P256(B, C, D, E, F, G, H, A, W[7], 0xAB1C5ED5);
            P256(A, B, C, D, E, F, G, H, W[8], 0xD807AA98);
            P256(H, A, B, C, D, E, F, G, W[9], 0x12835B01);
            P256(G, H, A, B, C, D, E, F, W[10], 0x243185BE);
            P256(F, G, H, A, B, C, D, E, W[11], 0x550C7DC3);
            P256(E, F, G, H, A, B, C, D, W[12], 0x72BE5D74);
            P256(D, E, F, G, H, A, B, C, W[13], 0x80DEB1FE);
            P256(C, D, E, F, G, H, A, B, W[14], 0x9BDC06A7);
            P256(B, C, D, E, F, G, H, A, W[15], 0xC19BF174);
            P256(A, B, C, D, E, F, G, H, R256(16), 0xE49B69C1);
            P256(H, A, B, C, D, E, F, G, R256(17), 0xEFBE4786);
            P256(G, H, A, B, C, D, E, F, R256(18), 0x0FC19DC6);
            P256(F, G, H, A, B, C, D, E, R256(19), 0x240CA1CC);
            P256(E, F, G, H, A, B, C, D, R256(20), 0x2DE92C6F);
            P256(D, E, F, G, H, A, B, C, R256(21), 0x4A7484AA);
            P256(C, D, E, F, G, H, A, B, R256(22), 0x5CB0A9DC);
            P256(B, C, D, E, F, G, H, A, R256(23), 0x76F988DA);
            P256(A, B, C, D, E, F, G, H, R256(24), 0x983E5152);
            P256(H, A, B, C, D, E, F, G, R256(25), 0xA831C66D);
            P256(G, H, A, B, C, D, E, F, R256(26), 0xB00327C8);
            P256(F, G, H, A, B, C, D, E, R256(27), 0xBF597FC7);
            P256(E, F, G, H, A, B, C, D, R256(28), 0xC6E00BF3);
            P256(D, E, F, G, H, A, B, C, R256(29), 0xD5A79147);
            P256(C, D, E, F, G, H, A, B, R256(30), 0x06CA6351);
            P256(B, C, D, E, F, G, H, A, R256(31), 0x14292967);
            P256(A, B, C, D, E, F, G, H, R256(32), 0x27B70A85);
            P256(H, A, B, C, D, E, F, G, R256(33), 0x2E1B2138);
            P256(G, H, A, B, C, D, E, F, R256(34), 0x4D2C6DFC);
            P256(F, G, H, A, B, C, D, E, R256(35), 0x53380D13);
            P256(E, F, G, H, A, B, C, D, R256(36), 0x650A7354);
            P256(D, E, F, G, H, A, B, C, R256(37), 0x766A0ABB);
            P256(C, D, E, F, G, H, A, B, R256(38), 0x81C2C92E);
            P256(B, C, D, E, F, G, H, A, R256(39), 0x92722C85);
            P256(A, B, C, D, E, F, G, H, R256(40), 0xA2BFE8A1);
            P256(H, A, B, C, D, E, F, G, R256(41), 0xA81A664B);
            P256(G, H, A, B, C, D, E, F, R256(42), 0xC24B8B70);
            P256(F, G, H, A, B, C, D, E, R256(43), 0xC76C51A3);
            P256(E, F, G, H, A, B, C, D, R256(44), 0xD192E819);
            P256(D, E, F, G, H, A, B, C, R256(45), 0xD6990624);
            P256(C, D, E, F, G, H, A, B, R256(46), 0xF40E3585);
            P256(B, C, D, E, F, G, H, A, R256(47), 0x106AA070);
            P256(A, B, C, D, E, F, G, H, R256(48), 0x19A4C116);
            P256(H, A, B, C, D, E, F, G, R256(49), 0x1E376C08);
            P256(G, H, A, B, C, D, E, F, R256(50), 0x2748774C);
            P256(F, G, H, A, B, C, D, E, R256(51), 0x34B0BCB5);
            P256(E, F, G, H, A, B, C, D, R256(52), 0x391C0CB3);
            P256(D, E, F, G, H, A, B, C, R256(53), 0x4ED8AA4A);
            P256(C, D, E, F, G, H, A, B, R256(54), 0x5B9CCA4F);
            P256(B, C, D, E, F, G, H, A, R256(55), 0x682E6FF3);
            P256(A, B, C, D, E, F, G, H, R256(56), 0x748F82EE);
            P256(H, A, B, C, D, E, F, G, R256(57), 0x78A5636F);
            P256(G, H, A, B, C, D, E, F, R256(58), 0x84C87814);
            P256(F, G, H, A, B, C, D, E, R256(59), 0x8CC70208);
            P256(E, F, G, H, A, B, C, D, R256(60), 0x90BEFFFA);
            P256(D, E, F, G, H, A, B, C, R256(61), 0xA4506CEB);
            P256(C, D, E, F, G, H, A, B, R256(62), 0xBEF9A3F7);
            P256(B, C, D, E, F, G, H, A, R256(63), 0xC67178F2);

            m_state[0] += A;
            m_state[1] += B;
            m_state[2] += C;
            m_state[3] += D;
            m_state[4] += E;
            m_state[5] += F;
            m_state[6] += G;
            m_state[7] += H;
        }

        void update(const unsigned char *input, const unsigned long int length)
        {
            unsigned long int left;
            unsigned long int fill;

            if(!length)
            {
                return;
            }
            
            unsigned long int len = length;

            left = m_total[0] & 0x3F;
            fill = 64 - left;

            m_total[0] += len;
            m_total[0] &= 0xFFFFFFFF;

            if(m_total[0] < len)
            {
                m_total[1]++;
            }

            if(left && len >= fill)
            {
                ::memcpy((void *) (m_buffer + left), (void *) input, fill);
                process(m_buffer);
                len -= fill;
                input += fill;
                left = 0;
            }

            while(len >= 64)
            {
                process(input);
                len -= 64;
                input += 64;
            }

            if(len)
            {
                ::memcpy((void *) (m_buffer + left), (void *) input, len);
            }
        }

        void finish(unsigned char digest[32])
        {
            static constexpr unsigned char sha256_padding[64] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            unsigned long int last, padn;
            unsigned long int high, low;
            unsigned char msglen[8];

            high = (m_total[0] >> 29) | (m_total[1] << 3);
            low = (m_total[0] << 3);

            PUT_UINT32(high, msglen, 0);
            PUT_UINT32(low, msglen, 4);

            last = m_total[0] & 0x3F;
            padn = (last < 56) ? (56 - last) : (120 - last);

            update(sha256_padding, padn);
            update(msglen, 8);

            PUT_UINT32(m_state[0], digest, 0);
            PUT_UINT32(m_state[1], digest, 4);
            PUT_UINT32(m_state[2], digest, 8);
            PUT_UINT32(m_state[3], digest, 12);
            PUT_UINT32(m_state[4], digest, 16);
            PUT_UINT32(m_state[5], digest, 20);
            PUT_UINT32(m_state[6], digest, 24);
            PUT_UINT32(m_state[7], digest, 28);
        }

    } sha256_context;

    //---------------------------------------
    // SHA1

    typedef struct
    {
    private:
        unsigned long int m_total[2];
        unsigned long int m_state[5];
        unsigned char m_buffer[64];
    public:

        void start()
        {
            m_total[0] = 0;
            m_total[1] = 0;

            m_state[0] = 0x67452301;
            m_state[1] = 0xEFCDAB89;
            m_state[2] = 0x98BADCFE;
            m_state[3] = 0x10325476;
            m_state[4] = 0xC3D2E1F0;
        }

        void process(const unsigned char data[64])
        {
            unsigned long int temp, W[16], A, B, C, D, E;

            GET_UINT32(W[0], data, 0);
            GET_UINT32(W[1], data, 4);
            GET_UINT32(W[2], data, 8);
            GET_UINT32(W[3], data, 12);
            GET_UINT32(W[4], data, 16);
            GET_UINT32(W[5], data, 20);
            GET_UINT32(W[6], data, 24);
            GET_UINT32(W[7], data, 28);
            GET_UINT32(W[8], data, 32);
            GET_UINT32(W[9], data, 36);
            GET_UINT32(W[10], data, 40);
            GET_UINT32(W[11], data, 44);
            GET_UINT32(W[12], data, 48);
            GET_UINT32(W[13], data, 52);
            GET_UINT32(W[14], data, 56);
            GET_UINT32(W[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define R1(t) (\
  temp = W[(t - 3) & 0x0F] ^ W[(t - 8) & 0x0F] ^ \
  W[(t - 14) & 0x0F] ^ W[t & 0x0F], \
  (W[t & 0x0F] = S(temp, 1))                         \
  )

#define P1(a,b,c,d,e,x) { e += S(a, 5) + F(b, c, d) + K + x; b = S(b, 30); }

            A = m_state[0];
            B = m_state[1];
            C = m_state[2];
            D = m_state[3];
            E = m_state[4];

#define F(x,y,z) (z ^ (x & (y ^ z)))
#define K 0x5A827999

            P1(A, B, C, D, E, W[0]);
            P1(E, A, B, C, D, W[1]);
            P1(D, E, A, B, C, W[2]);
            P1(C, D, E, A, B, W[3]);
            P1(B, C, D, E, A, W[4]);
            P1(A, B, C, D, E, W[5]);
            P1(E, A, B, C, D, W[6]);
            P1(D, E, A, B, C, W[7]);
            P1(C, D, E, A, B, W[8]);
            P1(B, C, D, E, A, W[9]);
            P1(A, B, C, D, E, W[10]);
            P1(E, A, B, C, D, W[11]);
            P1(D, E, A, B, C, W[12]);
            P1(C, D, E, A, B, W[13]);
            P1(B, C, D, E, A, W[14]);
            P1(A, B, C, D, E, W[15]);
            P1(E, A, B, C, D, R1(16));
            P1(D, E, A, B, C, R1(17));
            P1(C, D, E, A, B, R1(18));
            P1(B, C, D, E, A, R1(19));

#undef K
#undef F

#define F(x,y,z) (x ^ y ^ z)
#define K 0x6ED9EBA1

            P1(A, B, C, D, E, R1(20));
            P1(E, A, B, C, D, R1(21));
            P1(D, E, A, B, C, R1(22));
            P1(C, D, E, A, B, R1(23));
            P1(B, C, D, E, A, R1(24));
            P1(A, B, C, D, E, R1(25));
            P1(E, A, B, C, D, R1(26));
            P1(D, E, A, B, C, R1(27));
            P1(C, D, E, A, B, R1(28));
            P1(B, C, D, E, A, R1(29));
            P1(A, B, C, D, E, R1(30));
            P1(E, A, B, C, D, R1(31));
            P1(D, E, A, B, C, R1(32));
            P1(C, D, E, A, B, R1(33));
            P1(B, C, D, E, A, R1(34));
            P1(A, B, C, D, E, R1(35));
            P1(E, A, B, C, D, R1(36));
            P1(D, E, A, B, C, R1(37));
            P1(C, D, E, A, B, R1(38));
            P1(B, C, D, E, A, R1(39));

#undef K
#undef F

#define F(x,y,z) ((x & y) | (z & (x | y)))
#define K 0x8F1BBCDC

            P1(A, B, C, D, E, R1(40));
            P1(E, A, B, C, D, R1(41));
            P1(D, E, A, B, C, R1(42));
            P1(C, D, E, A, B, R1(43));
            P1(B, C, D, E, A, R1(44));
            P1(A, B, C, D, E, R1(45));
            P1(E, A, B, C, D, R1(46));
            P1(D, E, A, B, C, R1(47));
            P1(C, D, E, A, B, R1(48));
            P1(B, C, D, E, A, R1(49));
            P1(A, B, C, D, E, R1(50));
            P1(E, A, B, C, D, R1(51));
            P1(D, E, A, B, C, R1(52));
            P1(C, D, E, A, B, R1(53));
            P1(B, C, D, E, A, R1(54));
            P1(A, B, C, D, E, R1(55));
            P1(E, A, B, C, D, R1(56));
            P1(D, E, A, B, C, R1(57));
            P1(C, D, E, A, B, R1(58));
            P1(B, C, D, E, A, R1(59));

#undef K
#undef F

#define F(x,y,z) (x ^ y ^ z)
#define K 0xCA62C1D6

            P1(A, B, C, D, E, R1(60));
            P1(E, A, B, C, D, R1(61));
            P1(D, E, A, B, C, R1(62));
            P1(C, D, E, A, B, R1(63));
            P1(B, C, D, E, A, R1(64));
            P1(A, B, C, D, E, R1(65));
            P1(E, A, B, C, D, R1(66));
            P1(D, E, A, B, C, R1(67));
            P1(C, D, E, A, B, R1(68));
            P1(B, C, D, E, A, R1(69));
            P1(A, B, C, D, E, R1(70));
            P1(E, A, B, C, D, R1(71));
            P1(D, E, A, B, C, R1(72));
            P1(C, D, E, A, B, R1(73));
            P1(B, C, D, E, A, R1(74));
            P1(A, B, C, D, E, R1(75));
            P1(E, A, B, C, D, R1(76));
            P1(D, E, A, B, C, R1(77));
            P1(C, D, E, A, B, R1(78));
            P1(B, C, D, E, A, R1(79));

#undef K
#undef F

            m_state[0] += A;
            m_state[1] += B;
            m_state[2] += C;
            m_state[3] += D;
            m_state[4] += E;
        }

        void update(const unsigned char *input, const unsigned long int length)
        {
            unsigned long int left, fill;

            if(!length)
            {
                return;
            }
            unsigned long int len = length;

            left = m_total[0] & 0x3F;
            fill = 64 - left;

            m_total[0] += len;
            m_total[0] &= 0xFFFFFFFF;

            if(m_total[0] < len)
            {
                m_total[1]++;
            }

            if(left && len >= fill)
            {
                ::memcpy((void *) (m_buffer + left), (void *) input, fill);
                process(m_buffer);
                len -= fill;
                input += fill;
                left = 0;
            }

            while(len >= 64)
            {
                process(input);
                len -= 64;
                input += 64;
            }

            if(len)
            {
                ::memcpy((void *) (m_buffer + left), (void *) input, len);
            }
        }

        void finish(unsigned char digest[20])
        {
            static unsigned char sha1_padding[64] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            unsigned long int last, padn;
            unsigned long int high, low;
            unsigned char msglen[8];

            high = (m_total[0] >> 29) | (m_total[1] << 3);
            low = (m_total[0] << 3);

            PUT_UINT32(high, msglen, 0);
            PUT_UINT32(low, msglen, 4);

            last = m_total[0] & 0x3F;
            padn = (last < 56) ? (56 - last) : (120 - last);

            update(sha1_padding, padn);
            update(msglen, 8);

            PUT_UINT32(m_state[0], digest, 0);
            PUT_UINT32(m_state[1], digest, 4);
            PUT_UINT32(m_state[2], digest, 8);
            PUT_UINT32(m_state[3], digest, 12);
            PUT_UINT32(m_state[4], digest, 16);
        }

    } sha1_context;

    //---------------------------------------
    // MD5
    // MD5 uses a reverse order for Get and put so redefine them

#undef GET_UINT32
#undef PUT_UINT32

#define GET_UINT32(n,b,i) {                                               \
  (n) = ((unsigned long int)(b)[(i)])       \
  | ((unsigned long int)(b)[(i)+1] << 8)       \
  | ((unsigned long int)(b)[(i)+2] << 16)       \
  | ((unsigned long int)(b)[(i)+3] << 24);      \
    }

#define PUT_UINT32(n,b,i) {                                               \
  (b)[(i)] = (unsigned char)((n));       \
  (b)[(i)+1] = (unsigned char)((n) >> 8);       \
  (b)[(i)+2] = (unsigned char)((n) >> 16);       \
  (b)[(i)+3] = (unsigned char)((n) >> 24);       \
  }

    typedef struct
    {
        unsigned long int m_total[2];
        unsigned long int m_state[4];
        unsigned char m_buffer[64];

        void start()
        {
            m_total[0] = 0;
            m_total[1] = 0;

            m_state[0] = 0x67452301;
            m_state[1] = 0xEFCDAB89;
            m_state[2] = 0x98BADCFE;
            m_state[3] = 0x10325476;
        }

        void process(const unsigned char data[64])
        {
            unsigned long int X[16], A, B, C, D;

            GET_UINT32(X[0], data, 0);
            GET_UINT32(X[1], data, 4);
            GET_UINT32(X[2], data, 8);
            GET_UINT32(X[3], data, 12);
            GET_UINT32(X[4], data, 16);
            GET_UINT32(X[5], data, 20);
            GET_UINT32(X[6], data, 24);
            GET_UINT32(X[7], data, 28);
            GET_UINT32(X[8], data, 32);
            GET_UINT32(X[9], data, 36);
            GET_UINT32(X[10], data, 40);
            GET_UINT32(X[11], data, 44);
            GET_UINT32(X[12], data, 48);
            GET_UINT32(X[13], data, 52);
            GET_UINT32(X[14], data, 56);
            GET_UINT32(X[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P5(a,b,c,d,k,s,t)                                \
    {                                                       \
    a += F(b, c, d) + X[k] + t; a = S(a, s) + b;           \
    }

            A = m_state[0];
            B = m_state[1];
            C = m_state[2];
            D = m_state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

            P5(A, B, C, D, 0, 7, 0xD76AA478);
            P5(D, A, B, C, 1, 12, 0xE8C7B756);
            P5(C, D, A, B, 2, 17, 0x242070DB);
            P5(B, C, D, A, 3, 22, 0xC1BDCEEE);
            P5(A, B, C, D, 4, 7, 0xF57C0FAF);
            P5(D, A, B, C, 5, 12, 0x4787C62A);
            P5(C, D, A, B, 6, 17, 0xA8304613);
            P5(B, C, D, A, 7, 22, 0xFD469501);
            P5(A, B, C, D, 8, 7, 0x698098D8);
            P5(D, A, B, C, 9, 12, 0x8B44F7AF);
            P5(C, D, A, B, 10, 17, 0xFFFF5BB1);
            P5(B, C, D, A, 11, 22, 0x895CD7BE);
            P5(A, B, C, D, 12, 7, 0x6B901122);
            P5(D, A, B, C, 13, 12, 0xFD987193);
            P5(C, D, A, B, 14, 17, 0xA679438E);
            P5(B, C, D, A, 15, 22, 0x49B40821);

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

            P5(A, B, C, D, 1, 5, 0xF61E2562);
            P5(D, A, B, C, 6, 9, 0xC040B340);
            P5(C, D, A, B, 11, 14, 0x265E5A51);
            P5(B, C, D, A, 0, 20, 0xE9B6C7AA);
            P5(A, B, C, D, 5, 5, 0xD62F105D);
            P5(D, A, B, C, 10, 9, 0x02441453);
            P5(C, D, A, B, 15, 14, 0xD8A1E681);
            P5(B, C, D, A, 4, 20, 0xE7D3FBC8);
            P5(A, B, C, D, 9, 5, 0x21E1CDE6);
            P5(D, A, B, C, 14, 9, 0xC33707D6);
            P5(C, D, A, B, 3, 14, 0xF4D50D87);
            P5(B, C, D, A, 8, 20, 0x455A14ED);
            P5(A, B, C, D, 13, 5, 0xA9E3E905);
            P5(D, A, B, C, 2, 9, 0xFCEFA3F8);
            P5(C, D, A, B, 7, 14, 0x676F02D9);
            P5(B, C, D, A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x,y,z) (x ^ y ^ z)

            P5(A, B, C, D, 5, 4, 0xFFFA3942);
            P5(D, A, B, C, 8, 11, 0x8771F681);
            P5(C, D, A, B, 11, 16, 0x6D9D6122);
            P5(B, C, D, A, 14, 23, 0xFDE5380C);
            P5(A, B, C, D, 1, 4, 0xA4BEEA44);
            P5(D, A, B, C, 4, 11, 0x4BDECFA9);
            P5(C, D, A, B, 7, 16, 0xF6BB4B60);
            P5(B, C, D, A, 10, 23, 0xBEBFBC70);
            P5(A, B, C, D, 13, 4, 0x289B7EC6);
            P5(D, A, B, C, 0, 11, 0xEAA127FA);
            P5(C, D, A, B, 3, 16, 0xD4EF3085);
            P5(B, C, D, A, 6, 23, 0x04881D05);
            P5(A, B, C, D, 9, 4, 0xD9D4D039);
            P5(D, A, B, C, 12, 11, 0xE6DB99E5);
            P5(C, D, A, B, 15, 16, 0x1FA27CF8);
            P5(B, C, D, A, 2, 23, 0xC4AC5665);

#undef F

#define F(x,y,z) (y ^ (x | ~z))

            P5(A, B, C, D, 0, 6, 0xF4292244);
            P5(D, A, B, C, 7, 10, 0x432AFF97);
            P5(C, D, A, B, 14, 15, 0xAB9423A7);
            P5(B, C, D, A, 5, 21, 0xFC93A039);
            P5(A, B, C, D, 12, 6, 0x655B59C3);
            P5(D, A, B, C, 3, 10, 0x8F0CCC92);
            P5(C, D, A, B, 10, 15, 0xFFEFF47D);
            P5(B, C, D, A, 1, 21, 0x85845DD1);
            P5(A, B, C, D, 8, 6, 0x6FA87E4F);
            P5(D, A, B, C, 15, 10, 0xFE2CE6E0);
            P5(C, D, A, B, 6, 15, 0xA3014314);
            P5(B, C, D, A, 13, 21, 0x4E0811A1);
            P5(A, B, C, D, 4, 6, 0xF7537E82);
            P5(D, A, B, C, 11, 10, 0xBD3AF235);
            P5(C, D, A, B, 2, 15, 0x2AD7D2BB);
            P5(B, C, D, A, 9, 21, 0xEB86D391);

#undef F

            m_state[0] += A;
            m_state[1] += B;
            m_state[2] += C;
            m_state[3] += D;
        }

        void update(const unsigned char *input, const unsigned long int length)
        {
            unsigned long int left, fill;

            if(!length)
            {
                return;
            }
            unsigned long int len = length;

            left = m_total[0] & 0x3F;
            fill = 64 - left;

            m_total[0] += len;
            m_total[0] &= 0xFFFFFFFF;

            if(m_total[0] < len)
            {
                m_total[1]++;
            }

            if(left && len >= fill)
            {
                ::memcpy((void *) (m_buffer + left), (void *) input, fill);
                process(m_buffer);
                len -= fill;
                input += fill;
                left = 0;
            }

            while(len >= 64)
            {
                process(input);
                len -= 64;
                input += 64;
            }

            if(len)
            {
                ::memcpy((void *) (m_buffer + left), (void *) input, len);
            }
        }

        void finish(unsigned char digest[16])
        {
            static unsigned char md5_padding[64] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            unsigned long int last, padn;
            unsigned long int high, low;
            unsigned char msglen[8];

            high = (m_total[0] >> 29) | (m_total[1] << 3);
            low = (m_total[0] << 3);

            PUT_UINT32(low, msglen, 0);
            PUT_UINT32(high, msglen, 4);

            last = m_total[0] & 0x3F;
            padn = (last < 56) ? (56 - last) : (120 - last);

            update(md5_padding, padn);
            update(msglen, 8);

            PUT_UINT32(m_state[0], digest, 0);
            PUT_UINT32(m_state[1], digest, 4);
            PUT_UINT32(m_state[2], digest, 8);
            PUT_UINT32(m_state[3], digest, 12);
        }
    } md5_context;

    union State
    {

        State() { };

        State(State *ns)
        {
            ::memcpy(this, &ns, sizeof (State));
        }

        md5_context MD5;
        sha1_context SHA1;
        sha256_context SHA256;
    } m_state;

};

#endif /* KCHECKSUM_H */

