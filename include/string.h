/**
 * @file string.h
 * @brief String-related functions
 *
 */
#if !defined(_STRING_H)
#define _STRING_H

#include "common.h"

typedef u16 charcode;
typedef struct
{
    u8 dummy[8];
    charcode buffer[1];
} String;

#define HEAP_UNUSED 2 // ID for unused heap
#define _nweString ((String * (*)(u32, u32))0x02023131)
#define _deleteString ((void (*)(String *))0x02023105)

enum
{
    CHAR_SPACE = 1,
    CHAR_ZERO = 0xa2,
    CHAR_ONE,
    CHAR_TWO,
    CHAR_THREE,
    CHAR_FOUR,
    CHAR_FIVE,
    CHAR_SIX,
    CHAR_SEVEN,
    CHAR_EIGHT,
    CHAR_NINE,
    CHAR_TEN,
    CHAR_A,
    CHAR_B,
    CHAR_C,
    CHAR_D,
    CHAR_E,
    CHAR_F,
    CHAR_CR = 0xe000, // Carriage return (newline)
    CHAR_EOM = 0xffff, // End of message
};

// Function prototypes
static inline String *newString(u32 length);
static inline void deleteString(String *str);
static inline void u8toStrcode(u8 num, charcode *chars);
static inline void u32toStrcode(u32 num, charcode *chars);
static inline String *NumsToString(const u8 nums[], u32 size);
static inline String *NumsToStringWithSpace(const u8 nums[], u32 size);

/**
 * @brief Create a new string
 * @param length Maximum number of characters
 * @return Pointer to the created string
 */
static inline String *newString(u32 length)
{
    return _nweString(length, HEAP_UNUSED);
}

/**
 * @brief Delete a string
 * @param str Pointer to the string to be deleted
 */
static inline void deleteString(String *str)
{
    _deleteString(str);
}

/**
 * @brief Convert an u8 value to a hexadecimal charcode representation
 *
 * @param num Value to be converted
 * @param chars Buffer to receive the converted string (2 characters)
 */
static inline void u8toStrcode(u8 num, charcode *chars)
{
    u8 low = num & 0xF;
    u8 high = num >> 4;
    chars[0] = (high + CHAR_ZERO);
    chars[1] = (low + CHAR_ZERO);
}

/**
 * @brief Convert a u32 value to a hexadecimal charcode representation
 *
 * @param num Value to be converted
 * @param chars Buffer to receive the converted string (8 characters)
 */
static inline void u32toStrcode(u32 num, charcode *chars)
{
    u8 *_num = (u8 *)&num;
    for (int i = 0; i < 4; i++)
    {
        u8toStrcode(_num[3 - i], &chars[2 * i]);
    }
}

/**
 * @brief Convert a memory area to a hexadecimal string
 *
 * @param nums Pointer to the start of the memory area
 * @param size Size of the memory area (in bytes)
 * @return Pointer to the resulting string
 * @details No checks are performed on the number of characters
 */
static inline String *NumsToString(const u8 nums[], u32 size)
{
    String *str = newString(2 * size + 1);
    for (int i = 0; i < size; i++)
    {
        u8toStrcode(nums[i], &str->buffer[2 * i]);
    }
    str->buffer[2 * size] = CHAR_EOM;
    return str;
}

/**
 * @brief Convert a memory area to a hexadecimal string with spaces
 *
 * @param nums Pointer to the start of the memory area
 * @param size Size of the memory area (in bytes)
 * @return Pointer to the resulting string
 * @details No checks are performed on the number of characters
 */
static inline String *NumsToStringWithSpace(const u8 nums[], u32 size)
{
    String *str = newString(3 * size);
    for (int i = 0; i < size; i++)
    {
        u8toStrcode(nums[i], &str->buffer[3 * i]);
        str->buffer[3 * i + 2] = CHAR_SPACE;
    }
    str->buffer[3 * size - 1] = CHAR_EOM;
    return str;
}

#endif // _STRING_H
