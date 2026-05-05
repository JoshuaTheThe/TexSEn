
#ifndef SHEET_H
#define SHEET_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum
{
        NONE_T,
        INTEGER_T,
        REAL_T,
        STRING_T,
        EXPRESSION_T,
} TYPE;

typedef struct
{
        TYPE    Type;
        union {
        int     Integer;
        double  Real;
        char   *String;
        char   *Expression;
        } as;
} CELL;

typedef struct
{
        size_t Rows, Columns;
        CELL  *Cells;
} SHEET;

SHEET *CreateSheet(size_t W, size_t H);
void   DestroySheet(SHEET *Sheet);
void   UpdateSheet(SHEET *Sheet);
void   UpdateCell(SHEET *Sheet, size_t X, size_t Y, CELL NewValue);
CELL   ReadCell(SHEET *Sheet, size_t X, size_t Y);
CELL  *FindCell(SHEET *Sheet, size_t X, size_t Y);
void   DisplaySheet(SHEET *Sheet);

#endif

