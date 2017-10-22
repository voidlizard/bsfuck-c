#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strchunk.h"
#include "miscdata/slist.h"
#include "examples_common.h"

#define SPACES " \r\n\t"

static inline void emit_token(struct strchunk *ss, slist **to) {

    size_t len = strchunk_length(ss) + 2;

    unsigned char *str = example_alloc(0, len);
    slist *el = example_alloc(0, slist_size(len+2));

    assert( el );

    strchunk_cstr(ss, el->value, len);

/*    fprintf(stdout, "%s\n", el->value);*/

    slist *tmp = *to;
    tmp = slist_cons(el, tmp);
    *to = tmp;

    strchunk_destroy(ss, 0, example_dealloc);
}

static inline void skipspace(FILE *stream) {
    int c = ' ';
    for( ; !feof(stream) && strchr(SPACES, c); c = fgetc(stream));
    if( !feof(stream) && c != ' ' ) {
        ungetc(c, stream);
    }
}

bool read_token(FILE *stream, slist **tok) {

    struct strchunk *ss = strchunk_create(0, example_alloc);

    for(;;) {
        int c = fgetc(stream);
        if( c == EOF) {
            emit_token(ss, tok);
            return false;
        }
        if( strchr(SPACES, c) ) {
            emit_token(ss, tok);
            skipspace(stream);
            return true;
        }
        strchunk_append_char(ss, (char)c, 0, example_alloc);
    }

    return true;
}

int main(int argc, char **argv) {

    int c = 0;

    slist *tokens = 0;

    while( read_token(stdin, &tokens) );


    slist_reverse(&tokens);

    size_t n = slist_length(tokens);

    unsigned char **p = example_alloc(0, (n/2)*sizeof(unsigned char*));
    unsigned char **s = example_alloc(0, (n/2)*sizeof(unsigned char*));

    slist *it = tokens;
    size_t i = 0, j = 0, k = 0;
    for(; it; it = it->next, k++ ) {
        if( k % 2 == 0 ) {
            p[i++] = it->value;
        } else {
            s[j++] = it->value;
        }
    }

    for( i = 0; i < n/2; i++ ) {
        for( j = 0; j < n/2; j++ ) {
            fputs(p[i],stdout); //, "%s%s\n", p[i], s[j]);
            fputs(s[j],stdout); //, "%s%s\n", p[i], s[j]);
            fputc('\n', stdout);
        }
    }

}

