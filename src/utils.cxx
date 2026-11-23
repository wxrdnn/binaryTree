#include "h/utils.h"
#include "h/debug.h"
#include "string.h"
#include <cassert>
#include <cstddef>
#include <math.h>
#include <stdio.h>

const size_t cMaxLine = 1024;

void Swap(double *const a, double *const b)
{
    assert(a != NULL);
    assert(b != NULL);
    double t = *a;
    *a = *b;
    *b = t;
    return;
}

long CountLinesOfFile(FILE *const fp)
{
    assert(fp != NULL);
    long currentPos = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char buffer[cMaxLine] = {};
    long count = 0;
    while (fgets(buffer, cMaxLine, fp) != NULL)
    {
        ++count;
    }

    fseek(fp, currentPos, SEEK_SET);
    return count;
}

bool ReplaceChar(char *s, char target, char replacement)
{
    ASSERT(s);
    char *p = strchr(s, target);
    if (p)
    {
        *p = replacement;
        return true;
    }
    return false;
}

void ReplaceAllChars(char *s, char target, char replacement)
{
    ASSERT(s);

    while (ReplaceChar(s, target, replacement))
        s += strlen(s) + 1;
    return;
}

void ClearBuffer()
{
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    return;
}

bool FileIsEmpty(FILE *const fp)
{
    assert(fp != NULL);

    int c = 0; // default value
    if ((c = getc(fp)) == EOF)
    {
        return true;
    }

    ungetc(c, fp);
    return false;
}

void DumpIntArr(const int *const arr, const size_t size)
{
    fprintf(stderr, "DEBUG: Array dump: [");

    for (size_t i = 0; i < size; ++i)
    {
        fprintf(stderr, "%d", arr[i]);
        if (i < size - 1)
        {
            fprintf(stderr, ", ");
        }
    }

    fprintf(stderr, "]\n");
}

void DumpUnsignedIntArr(const unsigned *const arr, const size_t size)
{
    fprintf(stderr, "DEBUG: Array dump: [");

    for (size_t i = 0; i < size; ++i)
    {
        fprintf(stderr, "%u", arr[i]);
        if (i < size - 1)
        {
            fprintf(stderr, ", ");
        }
    }

    fprintf(stderr, "]\n");
}
