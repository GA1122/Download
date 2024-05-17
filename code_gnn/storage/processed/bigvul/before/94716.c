swabHorDiff16(TIFF* tif, uint8* cp0, tmsize_t cc)
{
    uint16* wp = (uint16*) cp0;
    tmsize_t wc = cc / 2;

    if( !horDiff16(tif, cp0, cc) )
        return 0;

    TIFFSwabArrayOfShort(wp, wc);
    return 1;
}