static OPJ_BOOL bmp_read_rle8_data(FILE* IN, OPJ_UINT8* pData,
                                   OPJ_UINT32 stride, OPJ_UINT32 width, OPJ_UINT32 height)
{
    OPJ_UINT32 x, y;
    OPJ_UINT8 *pix;
    const OPJ_UINT8 *beyond;

    beyond = pData + stride * height;
    pix = pData;

    x = y = 0U;
    while (y < height) {
        int c = getc(IN);

        if (c) {
            int j;
            OPJ_UINT8 c1 = (OPJ_UINT8)getc(IN);

            for (j = 0; (j < c) && (x < width) &&
                    ((OPJ_SIZE_T)pix < (OPJ_SIZE_T)beyond); j++, x++, pix++) {
                *pix = c1;
            }
        } else {
            c = getc(IN);
            if (c == 0x00) {  
                x = 0;
                ++y;
                pix = pData + y * stride + x;
            } else if (c == 0x01) {  
                break;
            } else if (c == 0x02) {  
                c = getc(IN);
                x += (OPJ_UINT32)c;
                c = getc(IN);
                y += (OPJ_UINT32)c;
                pix = pData + y * stride + x;
            } else {  
                int j;
                for (j = 0; (j < c) && (x < width) &&
                        ((OPJ_SIZE_T)pix < (OPJ_SIZE_T)beyond); j++, x++, pix++) {
                    OPJ_UINT8 c1 = (OPJ_UINT8)getc(IN);
                    *pix = c1;
                }
                if ((OPJ_UINT32)c & 1U) {  
                    getc(IN);
                }
            }
        }
    } 
    return OPJ_TRUE;
}
