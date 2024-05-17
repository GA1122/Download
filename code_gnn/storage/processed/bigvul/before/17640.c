swapStops(void *stuff, int num)
{
    int i;
    CARD32 *stops;
    CARD16 *colors;

    stops = (CARD32 *) (stuff);
    for (i = 0; i < num; ++i) {
        swapl(stops);
        ++stops;
    }
    colors = (CARD16 *) (stops);
    for (i = 0; i < 4 * num; ++i) {
        swaps(colors);
        ++colors;
    }
}
