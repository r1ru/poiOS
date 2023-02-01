#include "efi.h"
#include "common.h"


VOID putc(UINT16 c) {
    UINT16 str[2];
    str[0] = c;
    str[1] = L'\0';
    ST->ConOut->OutputtString(ST->ConOut, str);
}

VOID puts(UINT16 *s) {
    ST->ConOut->OutputtString(ST->ConOut, s);
}

UINT16 getc(VOID) {
    EFI_INPUT_KEY key;
    UINTN waitidx;

    ST->BootServices->WaitForEvent(1, &(ST->ConIn->WaitForKey), &waitidx); // キー入力を待つ

    while(ST->ConIn->ReadKeyStroke(ST->ConIn, &key));

    return key.UnicodeChar;
}

INT32 gets(UINT16 *buf, UINT16 buf_size) {
    INT32 i;
    
    for(i = 0; i < buf_size - 1;) {
        buf[i] = getc();
        putc(buf[i]);
        if(buf[i] == L'\r') {
            putc(L'\n');
            break;
        }
        i++;
    }
    
    buf[i] = L'\0'; // 最後に'\0'を付加、もしくはEOFを'\0'に置き換える
    
    return i;
}

// s1がs2に比べて1)小さい、2)等しい、3)大きい場合にゼロよりも1)小さい、2)等しい、3)大きい整数を返す。
INT32 strcmp(const UINT16 *s1, const UINT16 *s2) {
    char is_equal = 1;

    for(; (*s1 != L'\0' && *s2 != L'\0'); s1++, s2++) {
        if(*s1 != *s2) {
            is_equal = 0;
            break;
        }
    }

    if(is_equal) {
        if(*s1 != L'\0')
            return 1;
        else if(*s2 != L'\0')
            return -1;
        else 
            return 0;
    }else
        return (INT32)(*s1 - * s2);
    
}