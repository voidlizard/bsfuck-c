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

static inline bool read_token(FILE *stream, slist **tok) {

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

static inline char *dump(char **p, char c, bool force) {
    static char out[2*1024*1024];
    static char *ope = out+sizeof(out);

    if( *p == 0 ) {
        *p = out;
        return *p;
    }

    if( (*p)+1 >= ope || force ) {
        fwrite(out, 1, *p - out, stdout);
        *p = out;
    }

    **p = c;
    (*p)++;

    return *p;
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
    size_t i = 0, j = 0, k = 1;
    for(; it; it = it->next, k++ ) {
        if( k % 2 == 0 ) {
            p[i++] = it->value;
        } else {
            s[j++] = it->value;
        }
    }

    size_t sum = 0;

    char *pp = 0;
    dump(&pp, 0, false);

    for( i = 0; i < n/2; i++ ) {
        for( j = 0; j < n/2; j++ ) {

            char *sp = s[i];
            for( sp = s[i]; *sp;  sp++ ) {
                dump(&pp, *sp, false);
            }

            for( sp = p[j]; *sp;  sp++ ) {
                dump(&pp, *sp, false);
            }


            dump(&pp, '\n', false);
        }
    }

    dump(&pp, 0, true);

}

