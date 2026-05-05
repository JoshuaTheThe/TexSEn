
#include <sheet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SHEET  *CreateSheet(size_t W, size_t H)
{
        SHEET *NewSheet = calloc(1, sizeof(*NewSheet));
        if (!NewSheet) abort();
        NewSheet->Columns = W;
        NewSheet->Rows    = H;
        NewSheet->Cells   = calloc(W*H, sizeof(CELL));
        if (!NewSheet->Cells) abort();
        return NewSheet;
}

void    DestroySheet(SHEET *Sheet)
{
        for (size_t y = 0; y < Sheet->Rows; ++y)
                for (size_t x = 0; x < Sheet->Columns; ++x)
                        if (FindCell(Sheet, x, y)->Type == STRING_T)
                                free(FindCell(Sheet, x, y)->as.String);
        free(Sheet->Cells);
        free(Sheet);
}

void    UpdateSheetCell(SHEET *Sheet, size_t X, size_t Y)
{
        CELL *Cell = FindCell(Sheet, X, Y);
}

void    UpdateSheet(SHEET *Sheet)
{
        for (size_t y = 0; y < Sheet->Rows; ++y)
                for (size_t x = 0; x < Sheet->Columns; ++x)
                        UpdateSheetCell(Sheet, x, y);
}

void    UpdateCell(SHEET *Sheet, size_t X, size_t Y, CELL NewValue)
{
        if (Y >= Sheet->Rows)    return;
        if (X >= Sheet->Columns) return;
        if (Sheet->Cells[X + Y * Sheet->Columns].Type == STRING_T)
                free(Sheet->Cells[X + Y * Sheet->Columns].as.String);
        Sheet->Cells[X + Y * Sheet->Columns] = NewValue;
}

CELL   *FindCell(SHEET *Sheet, size_t X, size_t Y)
{
        if (Y >= Sheet->Rows)    return NULL;
        if (X >= Sheet->Columns) return NULL;
        return &Sheet->Cells[X + Y * Sheet->Columns];
}

CELL    ReadCell(SHEET *Sheet, size_t X, size_t Y)
{
        if (Y >= Sheet->Rows)    return (CELL){0};
        if (X >= Sheet->Columns) return (CELL){0};
        return Sheet->Cells[X + Y * Sheet->Columns];
}

void    PrintCellToBuffer(CELL *Cell, char *Buffer, size_t Max)
{
        if (Cell->Type == NONE_T)
               snprintf(Buffer,  Max, "  ");
        else if (Cell->Type == INTEGER_T)
                snprintf(Buffer, Max, "%d", Cell->as.Integer);
        else if (Cell->Type == REAL_T)
                snprintf(Buffer, Max, "%lf", Cell->as.Real);
        else if (Cell->Type == STRING_T)
                snprintf(Buffer, Max, "%s", Cell->as.String);
        else if (Cell->Type == EXPRESSION_T) // TODO - impl
                snprintf(Buffer, Max, "placeholder");
}

size_t  ComputeWidthPerCellForColumn(SHEET *Sheet, size_t x)
{
        size_t WPC = 0;
        char   Buffer[1024] = {0};
        for (size_t y = 0; y < Sheet->Rows; ++y)
        {
                CELL  *Cell  = FindCell(Sheet, x, y);
                memset(Buffer, 0, sizeof(Buffer));
                PrintCellToBuffer(Cell, Buffer, sizeof(Buffer) - 1);
                size_t Width = strnlen(Buffer, 1024);
                if (Width > WPC) WPC = Width;
        }
        memset(Buffer, 0, sizeof(Buffer));
        snprintf(Buffer, sizeof(Buffer) - 1, "%lx", x);
        size_t Width = strnlen(Buffer, 1024);
        if (Width > WPC) WPC = Width;
        return WPC;
}

size_t  WPR(SHEET *Sheet)
{
        char   Buffer[1024] = {0};
        snprintf(Buffer, sizeof(Buffer) - 1, "%lx", Sheet->Rows);
        return strnlen(Buffer, 1024);
}

void    DisplaySheet(SHEET *Sheet)
{
        printf("\n|%*x|", (int)WPR(Sheet), 0);
        for (size_t x = 0; x < Sheet->Columns; ++x)
        {
                size_t WPC = ComputeWidthPerCellForColumn(Sheet, x);
                printf("%*lx|", (int)WPC, x+1);
        }

        for (size_t y = 0; y < Sheet->Rows; ++y)
        {
                printf("\n|%*lx|", (int)WPR(Sheet), y + 1);
                for (size_t x = 0; x < Sheet->Columns; ++x)
                {
                        char   Buffer[1024] = {0};
                        CELL  *Cell  = FindCell(Sheet, x, y);
                        size_t WPC = ComputeWidthPerCellForColumn(Sheet, x);
                        PrintCellToBuffer(Cell, Buffer, sizeof(Buffer) - 1);
                        printf("%s", Buffer);
                        for (size_t t = strnlen(Buffer, 1024); t < WPC; ++t) printf(" ");
                        printf("|");
                }
        }
}

