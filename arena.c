#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    size_t cap;
    size_t size;
    char *data;
} Region;

size_t len(char *msg)
{
    size_t cum = 0;
    while(*msg != '\0'){
        ++cum;
        ++msg;
    }
    return cum;
}
void *regionAlloc(Region *r, size_t pos)
{
    assert(r->size + pos <= r->cap);
    void *res = (void *)r->data[r->size];
    r->size += pos;
    return res;
}
Region regionCreate(size_t cap)
{
    void *data = malloc(cap);
    assert(data != NULL);
    Region r = {
        .cap = cap,
        .data = data
    };
    return r;
}
void *regionWrite(Region *r, char *info)
{
    size_t start = r->size;
    size_t cum = len(info);
    size_t stop = r->size + cum + 1;
    size_t i;
    for(i = start; i < stop; ++i)
        r->data[i] = info[i-start];
    return r;
}
int main()
{
    // Trad malloc way of allocating mem
    int *Arena = malloc(256*sizeof(int));
    int i = 0;
    for(i; i < 128; ++i)
        Arena[i] = 0;
    for(i; i < 256; ++i)
        Arena[i] = 1;

    i = 0;
    for(i; i < 256; ++i)
        printf("%d ",Arena[i]);
    printf("\n\n");

    Arena[64] = 420;
    i = 0;
    for(i; i < 256; ++i)
        printf("%d ",Arena[i]);
    printf("\n");

    free(Arena);
    printf("\n---------------------------------------\n\n");
    // Region Allocation Example implementation
    char *msg = "Hello World!";
    char *new_msg = " I am very happy to meet you!";
    // Creating the Region
    Region r = regionCreate(256);
    Region *r_ptr = &r; // setting the pointer
    // Writing the first message
    regionWrite(r_ptr, msg);
    for(i = 0; i < len(msg); ++i)
        printf("%c",r.data[i]);
    printf("\n");
    // moving the Region pointer
    regionAlloc(r_ptr, len(msg));
    // Writing the second message
    regionWrite(r_ptr, new_msg);
    for(i = 0; i < len(msg) + len(new_msg); ++i)
        printf("%c",r.data[i]);
    printf("\n");
    // moving backward in memory
    regionAlloc(r_ptr,-4);
    // Overwriting data
    regionWrite(r_ptr, "**corrupted**");
    for(i = 0; i < len(msg) + len(new_msg); ++i)
        printf("%c",r.data[i]);
    printf("\n");
    // Testing the assert clause in regionAlloc
    regionAlloc(r_ptr, 249);
}
