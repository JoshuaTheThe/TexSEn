
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sheet.h>

void InputInteger(SHEET *Sheet, char *Buffer)
{
        int row, col;
        long long value;
        CELL NewValue = {0};
        if (sscanf(Buffer, "%*c %d %d %lld", &row, &col, &value) == 3)
        {
                NewValue.Type = INTEGER_T;
                NewValue.as.Integer = value;
                UpdateCell(Sheet, row, col, NewValue);
                printf("Set cell (%d,%d) to integer %lld\n", row, col, value);
        }
        else
        {
                printf("Invalid integer input format. Use: i row col value\n");
        }
}

void InputReal(SHEET *Sheet, char *Buffer)
{
        int row, col;
        double value;
        CELL NewValue = {0};
        if (sscanf(Buffer, "%*c %d %d %lf", &row, &col, &value) == 3)
        {
                NewValue.Type = REAL_T;
                NewValue.as.Real = value;
                UpdateCell(Sheet, row, col, NewValue);
                printf("Set cell (%d,%d) to real %f\n", row, col, value);
        }
        else
        {
                printf("Invalid real input format. Use: r row col value\n");
        }
}

void InputString(SHEET *Sheet, char *Buffer)
{
        int row, col;
        char stringValue[256];
        CELL NewValue = {0};
        char *token = Buffer + 1;
        while (*token == ' ' || *token == '\t')
                token++;
        if (sscanf(token, "%d %d", &row, &col) == 2)
        {
                char *stringStart = token;
                int numbersRead = 0;
                while (*stringStart && numbersRead < 2)
                {
                        if (*stringStart == ' ' || *stringStart == '\t')
                                numbersRead++;
                        stringStart++;
                }

                while (*stringStart == ' ' || *stringStart == '\t')
                        stringStart++;
                if (*stringStart == '"')
                {
                        stringStart++;
                        char *end = strchr(stringStart, '"');
                        if (end)
                        {
                                int len = end - stringStart;
                                if (len > 255)
                                        len = 255;
                                strncpy(stringValue, stringStart, len);
                                stringValue[len] = '\0';
                        }
                        else
                        {
                                strncpy(stringValue, stringStart, 255);
                                stringValue[255] = '\0';
                        }
                }
                else
                {
                        strncpy(stringValue, stringStart, 255);
                        stringValue[255] = '\0';
                        size_t len = strlen(stringValue);
                        if (len > 0 && stringValue[len - 1] == '\n')
                                stringValue[len - 1] = '\0';
                }

                NewValue.Type = stringValue[0] == '=' ? EXPRESSION_T : STRING_T;
                NewValue.as.String = strdup(stringValue + (stringValue[0] == '='));
                UpdateCell(Sheet, row, col, NewValue);
                printf("Set cell (%d,%d) to string \"%s\"\n", row, col, stringValue);
        }
        else
        {
                printf("Invalid string input format. Use: s row col \"string\" or s row col string\n");
        }
}

int main(void)
{
        SHEET *Sheet = CreateSheet(8, 8);
        char Buffer[1024] = {0};
        CELL NewValue = {0};
        NewValue.Type = STRING_T;
        NewValue.as.String = strdup("Hello");
        UpdateCell(Sheet, 2, 2, NewValue);
        NewValue.Type = INTEGER_T;
        NewValue.as.Integer = 42;
        UpdateCell(Sheet, 1, 2, NewValue);
        while (true)
        {
                memset(Buffer, 0, sizeof(Buffer));
                fgets(Buffer,
                      sizeof(Buffer) - 1,
                      stdin);
                if (strnlen(Buffer, 1024) == 0)
                        continue;
                // remove \n
                Buffer[strnlen(Buffer, 1024) - 1] = 0;
                if (Buffer[0] == 'q')
                        break;
                else if (Buffer[0] == 'd')
                        DisplaySheet(Sheet);
                else if (Buffer[0] == 'i')
                        InputInteger(Sheet, Buffer);
                else if (Buffer[0] == 'r')
                        InputReal(Sheet, Buffer);
                else if (Buffer[0] == 's')
                        InputString(Sheet, Buffer);
                /* would be nice to have
                else if (Buffer[0] == 'e')
                        InputExpression(Sheet,Buffer);
                else if (Buffer[0] == 'm')
                        MoveRegion(Sheet,Buffer);
                else if (Buffer[0] == 'y')
                        YankRegion(Sheet,Buffer);
                else if (Buffer[0] == 'p')
                        PasteRegion(Sheet,Buffer);*/
        }
        DestroySheet(Sheet);
        return 0;
}
