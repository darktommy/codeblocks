module main;

import std.stdio;
import std.algorithm;

char[] part1 = [
0x07, 0xC0, 0x01, 0x38, 0x43, 0xA9, 0x17, 0x38, 0x8D, 0x27, 0x96, 0x15, 0xA2, 0x1D, 0x24, 0x7B,
0x44, 0x80, 0x38, 0x10, 0x9F, 0x4C, 0xB7, 0x7F, 0x98, 0xF3, 0xB1, 0x22, 0x21, 0xEF, 0xA1, 0x6A,
0x1E, 0xE1, 0xB1, 0x26, 0xE7, 0x35, 0x18, 0xBE, 0x36, 0x0A, 0x3D, 0x30, 0xB9, 0x8E, 0xFA, 0x4A,
0xB0, 0xAC, 0x12, 0x34, 0x11, 0xF6, 0xBE, 0x4F, 0xF6, 0xA2, 0xD4, 0xEB, 0x6D, 0x4B, 0xA1, 0x70,
0xBB, 0x8F, 0x39, 0x39, 0x73, 0xE0, 0xE8, 0x15, 0x81, 0x90, 0xF1, 0x0A, 0xB4, 0x06, 0xCB, 0xD6,
0xD3, 0xD3, 0xF8, 0x5E, 0x15, 0xE2, 0x9B, 0xC8, 0xF9, 0x8A, 0x96, 0xCA, 0x7B, 0x43, 0xF5, 0x58,
0x19, 0x90, 0x45, 0xF6, 0x44, 0x8B, 0x0B, 0x7C, 0x00, 0x45, 0x1D, 0xDB, 0x3F, 0xD7, 0x36, 0x39,
];

char[] part2 = [
 0x1B, 0x23, 0x7C, 0x21, 0x75, 0x42, 0xF9, 0x68, 0x7D, 0xB0, 0xE7, 0x60, 0xE4, 0x1F, 0x87, 0x3A,
 0x6B, 0x23, 0x15, 0xFF, 0x34, 0xF6, 0x09, 0x02, 0x3A, 0xE7, 0xAD, 0x2A, 0xB4, 0x8D, 0x33, 0x29,
 0xF5, 0xBC, 0xA7, 0x94, 0xCF, 0x70, 0xF4, 0x87, 0xFA, 0x9D, 0x79, 0x7F, 0xD1, 0x90, 0x95, 0xB9,
 0x30, 0x06, 0xD9, 0x40, 0x75, 0xA5, 0xFC, 0x43, 0x70, 0x0B, 0xA2, 0x61, 0x87, 0xCD, 0x80, 0x81,
 0x09, 0x75, 0xDD, 0x19, 0x64, 0xFE, 0x8E, 0xEA, 0x9A, 0xF1, 0xD4, 0x0C, 0x04, 0x2C, 0xC9, 0x28,
 0x10, 0x99, 0x39, 0x1C, 0x92, 0x73, 0x47, 0xB5, 0x6E];

char[] part3 = [
0x16, 0xC2, 0x9E, 0x0C, 0x96, 0x90, 0xC7, 0xD5, 0x8B, 0x5D, 0x03, 0xF4, 0xE0, 0xA4, 0xF8, 0x29,
0x02, 0xFD, 0x98, 0x5F, 0x4D, 0x53, 0x99, 0xEB, 0x64, 0x69, 0x37, 0xE3, 0xEC, 0x53, 0xE4, 0x0C,
0x5B, 0xD1, 0x0A, 0x5C, 0x4B, 0xC2, 0xD4, 0x5C, 0x16, 0x3E, 0xBE, 0x1F, 0xC0, 0xE3, 0xBF];

const KEY_BEGIN = ' ';
const KEY_END = '~';
const MSG_BEGIN = ' ';
const MSG_END = '~';
const RAW_BEGIN = 0x00;
const RAW_END = 0xff;

void encode(char[] src, out char[] dst, char[] key)
{
    int i,j,k;
    dst.length = 0;
    for(i=0,j=1;i<src.length;i++)
    {
        k = i % cast(int)key.length;
        uint c;
        c = (key[k] * j) ^ src[i];
        dst ~= c & 0xFF;
        j += dst[i];
    }

}

void decode(char[] src, out char[] dst, char[] key)
{
    int i,j,k;
    dst.length = 0;
    for(i=0,j=1;i<src.length;i++)
    {
        k = i % cast(int)key.length;
        uint c;
        c = (key[k] * j) ^ src[i];
        dst ~= c & 0xFF;
        j += src[i];
    }
}

int hash(char[] src)
{
    int len = cast(int)src.length;
    int i;
    int hashcode = 1;
    for(i=0;i<len;i++)
    {
        hashcode *= cast(int)src[i];
        hashcode += cast(int)src[(i+1)%len];
        hashcode ^= i;
    }
    return hashcode;
}

int check(char[] key)
{
    char buf[];
    uint hsh = 0;
    int keylen = cast(int)key.length;
    decode(part3, buf, key);
    hsh = hash(buf);

    if(hsh == 0xF16F28CA)
        return 1;
    return 0;
}

int check2(char[] key)
{
    char[] buf1,buf2;
    uint hsh1=0,hsh2=0;

    decode(part1,buf1,key);
    decode(part2,buf2,key);

    hsh1 = hash(buf1);
    hsh2 = hash(buf2);

    if(hsh1+hsh2 == 0x6150ECA6)
    {
        return 1;
    }
    return 0;
}

void print_array(char array[][])
{
    int i,j;
    for(i=0;i<array.length;i++)
    {
        for(j=0;j<array[i].length;j++)
        {
            writef("%c ", array[i][j]);
        }
        writefln("[%d] .%d", array[i].length, i);
    }
}

void remove_doubles(ref char keys[][])
{
    int i,j,k;
    int flag;
    char ch;
    char newkeys[][];
    newkeys.length = keys.length;

    for(i=0;i<keys.length;i++)
    {
        for(j=0;j<keys[i].length;j++)
        {
            flag = 1;
            ch = keys[i][j];
            for(k=0;k<newkeys[i].length;k++)
            {

                if(ch == newkeys[i][k])
                {
                    flag = 0;
                    break;
                }
            }
            if(flag)
            {
                newkeys[i] ~= ch;
            }
        }
    }

    keys.length = newkeys.length;
    for(i=0;i<newkeys.length;i++)
    {
        keys[i].length = 0;
        for(k=0;k<newkeys[i].length;k++)
        {

            keys[i] ~= newkeys[i][k];
        }
    }
}


void cyclic_check(char keys[][], int pwdlen)
{
    char newkeys[][];
    newkeys.length = pwdlen;

    int start;
    int i,j,k;
    int flag, num;
    char ch;

    for(start = 0; start<pwdlen;start++)
    {
        for(i=0;i<keys[start].length;i++)
        {
            ch = keys[start][i];
            if(pwdlen+start >= keys.length)
            {
                newkeys[start] ~= ch;
                continue;
            }


            for(j=pwdlen+start;j<keys.length;j+=pwdlen)
            {
                flag = 0;
                for(k=0;k<keys[j].length;k++)
                {
                    if(keys[j][k] == ch)
                    {
                        flag = 1;
                        break;
                    }
                }
            }
            if(flag)
            {
                newkeys[start] ~= ch;
            }

        }
    }

    keys.length = newkeys.length;
    for(i=0;i<newkeys.length;i++)
    {
        keys[i].length = 0;
        for(j=0;j<newkeys[i].length;j++)
        {
            keys[i] ~= newkeys[i][j];
        }
    }
}


ulong get_variations(char[][]keys)
{
    int i;
    ulong variations=1;

    for(i=0;i<keys.length;i++)
    {
        variations *= keys[i].length;
    }

    return variations;
}

ulong get_variations(char[][]keys, int pwdlen)
{
    int i;
    ulong variations=1;

    for(i=0;i<pwdlen;i++)
    {
        variations *= keys[i].length;
    }

    return variations;
}

int brute_routine(int depth, char vars[][], int iter, char[] key)
{
    int i;
    if(iter == depth)
    {
        for(i = 0;i<vars[iter].length;i++)
        {
            key[iter] = vars[iter][i];
            if(check(key))
            {
                writefln("check1 done! key = %s", key);
                if(check2(key))
                {
                    writefln("oh yeeeah.. key = %s", key);
                    return 1;
                }
            }
        }
        return 0;
    }
    else
    {
        for(i=0;i<vars[iter].length;i++)
        {
            key[iter] = vars[iter][i];
            brute_routine(depth, vars, iter+1, key);
        }
    }
    return 0;
}

void bruteforce(int depth, char vars[][])
{
    char[] key;
    key.length = depth+1;

    brute_routine(depth, vars, 0, key);

}

void bingo()
{
    uint key_c,src_c;
    uint c;
    int i,k;
    uint j;
    uint[] mult1,mult2,mult3;
    char[][] key1,key2,key3;
    key1.length = part1.length;
    key2.length = part2.length;
    key3.length = part3.length;
    int pwdlen = 25;

    j=1;
    for(i=0;i<part1.length;i++)
    {
        mult1 ~= j;
        j = j + cast(ubyte)part1[i];
    }
    j=1;
    for(i=0;i<part2.length;i++)
    {
        mult2 ~= j;
        j = j + cast(ubyte)part2[i];
    }
    j=1;
    for(i=0;i<part3.length;i++)
    {
        mult3 ~= j;
        writefln("%d [%d]",j,i);
        j = j + cast(ubyte)part3[i];
    }


    char ch;
    for(i=0;i<part1.length;i++)
    {
        for(key_c=KEY_BEGIN;key_c<=KEY_END;key_c++)
        {
            for(src_c=MSG_BEGIN;src_c<=MSG_END;src_c++)
            {
                c = (key_c * mult1[i]) ^ src_c;
                ch = c & 0xff;

                if(ch == part1[i])
                {
                    key1[i] ~= key_c;
                }
            }
        }
    }

    for(i=0;i<part2.length;i++)
    {
        for(key_c=KEY_BEGIN;key_c<=KEY_END;key_c++)
        {
            for(src_c=MSG_BEGIN;src_c<=MSG_END;src_c++)
            {
                c = (key_c * mult2[i]) ^ src_c;
                ch = c & 0xff;

                if(ch == part2[i])
                {
                    key2[i] ~= key_c;
                }
            }
        }
    }

    for(i=0;i<part3.length;i++)
    {
        for(key_c=KEY_BEGIN;key_c<=KEY_END;key_c++)
        {
            for(src_c=MSG_BEGIN;src_c<=MSG_END;src_c++)
            {

                c = (key_c * mult3[i]) ^ src_c;
                ch = c & 0xff;

                if(ch == part3[i])
                {
                    key3[i] ~= key_c;
                }
            }
        }
    }
    writeln("KEY3:");;
    print_array(key3);

    remove_doubles(key1);
    remove_doubles(key2);
    remove_doubles(key3);

    cyclic_check(key1,pwdlen);
    cyclic_check(key2,pwdlen);
    cyclic_check(key3,pwdlen);

    writeln("KEY1:");
    print_array(key1);

    writeln("KEY2:");
    print_array(key2);

    writeln("KEY3:");;
    print_array(key3);

    char tmp_keys[][];
    tmp_keys.length = pwdlen;
    int flag;

    for(i=0;i<pwdlen;i++)
    {
        for(j=0;j<key1[i].length;j++)
        {
            ch = key1[i][j];

            //ищем этот символ в key1
            flag = 0;
            for(k=0;k<key2[i].length;k++)
            {
                if(ch == key2[i][k])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0) continue;

            //и в key2
            flag = 0;
            for(k=0;k<key3[i].length;k++)
            {
                if(ch == key3[i][k])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0) continue;

            tmp_keys[i] ~= ch;
        }
    }

    writefln("\n\nUsing theese characters...");
    print_array(tmp_keys);
    writefln("Variations: %d", get_variations(tmp_keys));

    bruteforce(pwdlen-1,tmp_keys);
}

int main(string[] args)
{

    bingo();
	return 0;
}
