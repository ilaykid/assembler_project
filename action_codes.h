#ifndef ACTION_CODES_H
#define ACTION_CODES_H

typedef struct {
    int code;
     char* name;
} ActionCode;

static  ActionCode action_codes[] = {
    {0, "mov"},
    {1, "cmp"},
    {2, "add"},
    {3, "sub"},
    {4, "not"},
    {5, "clr"},
    {6, "lea"},
    {7, "inc"},
    {8, "dec"},
    {9, "jmp"},
    {10, "bne"},
    {11, "red"},
    {12, "prn"},
    {13, "jsr"},
    {14, "rts"},
    {15, "stop"},
};

#endif // ACTION_CODES_H
