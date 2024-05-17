BitsClear(CARD32 data)
{
    int bits = 0;
    CARD32 mask;

    for (mask = 1; mask; mask <<= 1) {
        if (!(data & mask))
            bits++;
        else
            break;
    }

    return bits;
}
