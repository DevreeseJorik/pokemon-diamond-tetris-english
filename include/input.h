/**
 * @file input.h
 * @brief Key input related
 *
 */

#if !defined(_IPUT_H)
#define _IPUT_H

#define pad_state (*(u32 *)(0x021c6118 + 0x30))         // Whether a key is pressed
#define pad_state_keydown (*(u32 *)(0x021c6118 + 0x3c)) // Whether a key was pressed in this frame


#define pad_keyA 0x0001
#define pad_keyB 0x0002
#define pad_keySelect 0x0004
#define pad_keyStart 0x0008
#define pad_keyRight 0x0010
#define pad_keyLeft 0x0020
#define pad_keyUp 0x0040
#define pad_keyDown 0x0080
#define pad_keyR 0x0100
#define pad_keyL 0x0200
#define pad_keyX 0x0400
#define pad_keyY 0x0800

#endif // _IPUT_H
