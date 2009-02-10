#include <math.h>
#include <stdio.h>
#include <stdlib.h>

char* itostr(int val)
{
    char *str, len = 0, i, sign = 0;

    if (val < 0) {
        len += 1;
        val = 0 - val;
        sign = 1;
    }

    len += log10(val);
    len += 2;

    str = malloc(len * sizeof(char));
    if (!str) {
        printf("Can not malloc memory \n");
        return 0;
    }

    if (sign)
        str[0] = '-';

    for (i = sign; i < len - 1; i++) {
        int temp;
        temp = pow(10, len - 2 - i);
        str[i] = (char) (val / temp + '0');
        val = val % temp;
    }
    str[len - 1] = '\0';
    return str;
}
