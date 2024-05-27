
size_t digits_to_hex_string(long digits, char* hex_string)
{
    size_t i, j, k, digit_count;
    long q, r;

    i = 0;

    if (digits == 0) {
        hex_string[i++] = 0x30; // avoids integer overflows < if removed fix k = (i - 1) >
    }

    for (digit_count = 0; i < (BIG_ENOUGH - 10) && digits != 0; digit_count++) // Arbitrary value of 10 and BIG_ENOUGH
    {
        if (i != 0 && (digit_count % 2 == 0)) {
            hex_string[i++] = ',';
        }

        q = digits >> 4;
        digits -= (q << 4);

        switch (digits) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                hex_string[i++] = (digits + 0x30);
                break;
            case 10:
                hex_string[i++] = 'a';
                break;
            case 11:
                hex_string[i++] = 'b';
                break;
            case 12:
                hex_string[i++] = 'c';
                break;
            case 13:
                hex_string[i++] = 'd';
                break;
            case 14:
                hex_string[i++] = 'e';
                break;
            case 15:
                hex_string[i++] = 'f';
                break;
            default: // wut ?
                break;
        }
        if (digit_count % 2 != 0) {
            hex_string[i++] = 'x';
            hex_string[i++] = '0';
        }

        digits = q;
    }

    hex_string[i++] = ' ';
    hex_string[i++] = 'b';
    hex_string[i++] = 'd';

    j = 0;
    k = (i - 1);
    for (;j < k; j++, k--) {
        hex_string[j] ^= hex_string[k];
        hex_string[k] ^= hex_string[j];
        hex_string[j] ^= hex_string[k];
    }

    hex_string[i++] = '\n';
    hex_string[i] = '\0';

    return i;
}
