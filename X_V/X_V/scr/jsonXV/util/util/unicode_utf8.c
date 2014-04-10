//
//  unicode_utf8.c
//  X_V
//
//  Created by zouxu on 6/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#include <stdio.h>
/*
 * ===========================================================================
 *
 *       Filename:  unicode_utf-8.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2011年12月23日 15时23分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  nuoerlz (nuoliu), nuoerlz@gmail.com
 *        Company:  mhtt
 *
 *      CopyRight:  Reserve
 *
 * ===========================================================================
 */

/*
 * Unicode 与 UTF-8 的关系:
 *
 *   Unicode 编码                     UTF-8 存储码
 * ========================  =====================================================
 * U-00000000 - U-0000007F:  0xxxxxxx
 * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
 * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
 ====   ====--   ------
 * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 +++   ++====   ====--   ------
 * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 ^^   ++++++   ++====   ====--   ------
 * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 ^   ^^^^^^   ++++++   ++====   ====--   ------
 *
 *  (前面几个1就代表后面几个字节是属于一起的)
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#include <alg_table.h>


/*****************************************************************************
 * 根据字符的UTF-8编码的第一个字节求出该字符用UTF-8编码存储时所需要多少个
 * 字节空间. 特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
 *
 * 参数:
 *    c    字符的UTF-8编码的第一个字节的值
 *
 * 返回值:
 *   该字符用UTF-8编码存储时所需要多少个字节空间.
 *   特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
 ****************************************************************************/
int enc_get_utf8_size(unsigned char c)
{
    unsigned char t = 0x80;
    unsigned char r = c;
    int count = 0;
    
    while ( r & t )
    {
        r = r << 1;
        count++;
    }
    
    return count;
}

/*****************************************************************************
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码
 *    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                类型为unsigned long .
 *
 * 返回值:
 *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned short *Unic)
{
    assert(pInput != NULL && Unic != NULL);
    
    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;
    
    *Unic = 0x0; // 把 *Unic 初始化为全零
    int utfbytes = enc_get_utf8_size(*pInput);
    unsigned char *pOutput = (unsigned char *) Unic;
    
    switch ( utfbytes )
    {
        case 0:
            *pOutput     = *pInput;
            utfbytes    += 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xE0) != 0x80 )
                return 0;
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                || ((b4 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                || ((b6 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
            break;
    }
    
    return utfbytes;
}

/*****************************************************************************
 * 将字符串的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码
 *    nMembIn     pInput大小(即, 字符串的长度)(单位: 1字节)
 *    pOutput     指向输出缓冲区, 用于保存Unicode编码值
 *    nMembOut    输入: pOutput可以存储的Unicode编码(字符)的个数(单位: 4字节);
 *                输出: 成功转换的字符的个数.
 *
 * 返回值:
 *    若有错误发生, 则返回 0 ;
 *    若所有的字符都转换成功, 则返回 1 ;
 *    若输出缓冲区空间不足, 导致只有部分字符转换成功, 则返回 2 ;
 *    另: *nMembOut 返回成功转换的字符的个数.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_utf8_to_unicode(const unsigned char* pInput, int nMembIn,
                        unsigned short* pOutput, int* nMembOut)
{
    assert(pInput != NULL && pOutput != NULL);
    assert(*nMembOut >= 1);
    
    int i, ret, outSize;
    const unsigned char *pIn     = pInput;
    unsigned short       *pOutCur = pOutput;
    
    outSize = *nMembOut;
    for ( i = 0; i < nMembIn; )
    {
        if ( pOutCur - pOutput >= outSize )
        {
            *nMembOut = pOutCur - pOutput;
            return 2; // 输出空间不足
        }
        
        ret = enc_utf8_to_unicode_one(pIn, pOutCur);
        if ( ret == 0 )
        {
            *nMembOut = pOutCur - pOutput;
            return 0; // 输入的字符编码不是UTF8
        }
        
        i    += ret;
        pIn  += ret;
        pOutCur += 1;
    }
    *pOutCur=0;
    *nMembOut = pOutCur - pOutput;
    return 1;         // 所有字符转换成功
}

/*****************************************************************************
 * 将以0结束的字符串的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码, 以0结束.
 *    pOutput     指向输出缓冲区, 用于保存Unicode编码值
 *    nMembOut    输入: pOutput可以存储的Unicode编码(字符)的个数(单位: 4字节);
 *                输出: 成功转换的字符的个数.
 *
 * 返回值:
 *    若有错误发生, 则返回 0 ;
 *    若所有的字符都转换成功, 则返回 1 ;
 *    若输出缓冲区空间不足, 导致只有部分字符转换成功, 则返回 2 ;
 *    另: *nMembOut 返回成功转换的字符的个数.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_utf8_to_unicode_str(const unsigned char *pInput,
                            unsigned short *pOutput, int *nMembOut)
{
    assert(pInput != NULL && pOutput != NULL);
    //assert(*nMembOut >= 1);
    if(*nMembOut <= 0)return 1;
    
    //int strLen = strlen((const char *)pInput);
   // printf("strLen: %d", strLen);
    
    return enc_utf8_to_unicode(pInput, strlen((const char *)pInput),
                               pOutput, nMembOut);
}

/*****************************************************************************
 * 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
 *
 * 参数:
 *    unic     字符的Unicode编码值
 *    pOutput  指向输出的用于存储UTF8编码值的缓冲区的指针
 *    outsize  pOutput缓冲的大小
 *
 * 返回值:
 *    返回转换后的字符的UTF8编码所占的字节数, 如果出错则返回 0 .
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!
 ****************************************************************************/
int enc_unicode_to_utf8_one(const unsigned short * pIn/*unsigned short  unic*/, unsigned char *pOutput,
                            int outSize)
{
    assert(pOutput != NULL);
    assert(outSize >= 6);
    
    unsigned int  unic = *pIn;//read a short, otherwise set a int to it
//    if(unic > 0x0000FFFF
    unsigned int  shortTwo = *((unsigned int * )pIn);
   // printf("len: %x-%x\n", unic, shortTwo);
    
    if ( unic <= 0x0000007F )
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        *pOutput     = (unic & 0x7F);
        pIn+=1;
        return 1;
    }
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        *(pOutput+1) = (unic & 0x3F) | 0x80;
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
         pIn+=1;
        return 2;
    }
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput+2) = (unic & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
         pIn+=1;
        return 3;
    }
    else{
        unic = shortTwo;// !!! this section is not tested, by_zouxu
        if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+3) = (unic & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        pIn+=2;
        return 4;
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+4) = (unic & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        pIn+=2;
        return 5;
    }
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+5) = (unic & 0x3F) | 0x80;
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        pIn+=2;
        return 6;
    }
    }
    
    return 0;
}

/*****************************************************************************
 * 将字符串的Unicode(UCS-2和UCS-4)编码转换成UTF8编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以Unicode编码
 *    nMembIn     pInput大小(即, 字符串的长度)(单位: 4字节)
 *    pOutput     指向输出缓冲区, 用于保存UTF8编码值
 *    nMembOut    输入: pOutput缓冲区的大小(单位: 1字节);
 *                输出: 成功转换后, UTF8编码所占空间大小(单位: 1字节).
 *
 * 返回值:
 *    若有错误发生, 则返回 0 ;
 *    若所有的字符都转换成功, 则返回 1 ;
 *    若输出缓冲区空间不足, 导致只有部分字符转换成功, 则返回 2 ;
 *    另: *nMembOut返回UTF8编码所占空间大小(单位: 1字节).
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_unicode_to_utf8(unsigned short *pInput, int nMembIn,
                        unsigned char *pOutput, int *nMembOut)
{
    assert(pInput != NULL && pOutput != NULL );
    assert(*nMembOut >= 6);
    
    int i, ret, outSize, resOutSize;
    const unsigned short *pIn     = pInput;
    unsigned char       *pOutCur = pOutput;
    
    outSize = *nMembOut;
    for ( i = 0; i < nMembIn; )
    {
        resOutSize = outSize - (pOutCur - pOutput);
        if ( resOutSize < 6 )
        {
            *nMembOut = pOutCur - pOutput;
            return 2;
        }
        
        ret = enc_unicode_to_utf8_one(pIn, pOutCur, resOutSize);
        if ( ret == 0 )
        {
            *nMembOut = pOutCur - pOutput;
            return 0;
        }
        
        i   += 1;
        pIn += 1;
        pOutCur += ret;
    }
    
    *nMembOut = pOutCur - pOutput;
    return 1;
}

/*****************************************************************************
 * 将字符串的Unicode(UCS-2和UCS-4)编码(以0结束)转换成UTF8编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以Unicode编码, 以0结束.
 *    pOutput     指向输出缓冲区, 用于保存UTF8编码值
 *    nMembOut    输入: pOutput缓冲区的大小(单位: 1字节);
 *                输出: 成功转换后, UTF8编码所占空间大小(单位: 1字节).
 *
 * 返回值:
 *    若有错误发生, 则返回 0 ;
 *    若所有的字符都转换成功, 则返回 1 ;
 *    若输出缓冲区空间不足, 导致只有部分字符转换成功, 则返回 2 ;
 *    另: *nMembOut返回UTF8编码所占空间大小(单位: 1字节).
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_unicode_to_utf8_str(const unsigned short *pInput,
                            unsigned char *pOutput, int *nMembOut)
{
    assert(pInput != NULL && pOutput != NULL );
    assert(*nMembOut >= 6);
    
    int ret, outSize, resOutSize;
    const unsigned short *pIn     = pInput;
    unsigned char       *pOutCur = pOutput;
    
    outSize = *nMembOut;
    for ( ; *pIn != 0; )
    {
        resOutSize = outSize - (pOutCur - pOutput);
        if ( resOutSize < 6 )
        {
            *nMembOut = pOutCur - pOutput;
            return 2;
        }
        
        ret = enc_unicode_to_utf8_one(pIn, pOutCur, resOutSize);
       // printf("len: %d\n", ret);
        if ( ret == 0 )
        {
            *nMembOut = pOutCur - pOutput;
            return 0;
        }
        
        pIn += 1;
        pOutCur += ret;
    }
    *pOutCur=0;
    *nMembOut = pOutCur - pOutput;
    return 1;
}

