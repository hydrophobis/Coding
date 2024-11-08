#include <stdio.h>
#include <string.h>

#define boolacc(b) b.field
#define boolcons(val) ((bool){val})
struct bool {
    unsigned int field : 1;
};
typedef struct bool bool;
void bprint(bool in) {
    if(boolacc(in) == 1) {
        printf("true");
    } else if(boolacc(in) == 0) {
        printf("false");
    } else {
        printf("invalid boolean value");
    }
}

#define stracc(s) s.field
#define strcons(s) {s}
struct string {
    char *field;
};
typedef struct string string;
bool contains_char(const struct string *s, const char ch) {
    bool out = boolcons(1);
    int len = strlen(s->field);  // Store length once
    for (int i = 0; i < len; i++) {
        if (s->field[i] == ch) {
            return out;
        }
    }
    out = boolcons(0);
    return out;
}
bool contains_any(const struct string *s, const char *chars) {
    bool out = boolcons(1);
    for (int i = 0; i < strlen(s->field); i++) {
        for (int j = 0; j < strlen(chars); j++) {
            if (s->field[i] == chars[j]) {
                return out;
            }
        }
    }
    out = boolcons(0);
    return out;
}
