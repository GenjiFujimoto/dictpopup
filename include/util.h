#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <glib.h>

typedef uint8_t   u8;
typedef int32_t   i32;
typedef uint32_t  u32;
typedef ptrdiff_t size;

#define Stopif(assertion, error_action, ...)          \
	if (assertion) {                              \
		notify(1, __VA_ARGS__);               \
		fprintf(stderr, __VA_ARGS__);         \
		fprintf(stderr, "\n");                \
		error_action;                         \
	}

#define assert(c)     while (!(c)) __builtin_unreachable()

/*
 * A malloc wrapper, which aborts on insufficient memory
 */ 
void* _malloc(size_t size);
void* _calloc(size_t nmemb, size_t size);
void* _realloc(void* ptr, size_t size);
#define new(type, num) _calloc(num, sizeof(type))

#define countof(a)    (size)(sizeof(a) / sizeof(*(a)))
#define lengthof(s)   (countof("" s "") - 1)
#define s8(s)         (s8){(u8 *)s, countof(s)-1}
typedef struct {
    u8  *s;
    size len;
} s8;

void u8copy(u8 *dst, u8 *src, size n);
i32 u8compare(u8 *a, u8 *b, size n);
s8 s8copy(s8 dst, s8 src);
s8 s8dup(s8 s);
s8 s8fromcstr(char *z);
i32 s8equals(s8 a, s8 b);
void s8striputf8chr(s8* s);
s8 s8unescape(s8 str);


typedef struct {
  char *dictname;
  char *kanji;
  char *reading;
  char *definition;
} dictentry;

dictentry dictentry_dup(dictentry de);
void dictentry_free(void* ptr);
void dictentry_print(dictentry de);


typedef GPtrArray dictionary;

dictionary* dictionary_new(void);
void dictionary_copy_add(dictionary* dict, dictentry de);
void dictionary_free(dictionary* dict);
dictentry dictentry_at_index(dictionary *dict, unsigned int index);
typedef int (*DictComparer)(dictentry* a, dictentry* b);
void dictionary_sort(dictionary* dict, DictComparer compare_func);
void dictionary_print(dictionary* dict);


void notify(bool urgent, char const *fmt, ...);

#include "ankiconnectc.h"
bool check_ac_response(retval_s ac_resp);

char* str_repl_by_char(char *str, char *target, char repl_c);
void nuke_whitespace(char *str);

char* read_cmd_sync(char** argv);
int printf_cmd_async(char const *fmt, ...);
int printf_cmd_sync(char const *fmt, ...);

#endif
