local uLong crc32_combine_(crc1, crc2, len2)
    uLong crc1;
    uLong crc2;
    z_off64_t len2;
{
    int n;
    unsigned long row;
    unsigned long even[GF2_DIM];     
    unsigned long odd[GF2_DIM];      

     
    if (len2 <= 0)
        return crc1;

     
    odd[0] = 0xedb88320UL;           
    row = 1;
    for (n = 1; n < GF2_DIM; n++) {
        odd[n] = row;
        row <<= 1;
    }

     
    gf2_matrix_square(even, odd);

     
    gf2_matrix_square(odd, even);

     
    do {
         
        gf2_matrix_square(even, odd);
        if (len2 & 1)
            crc1 = gf2_matrix_times(even, crc1);
        len2 >>= 1;

         
        if (len2 == 0)
            break;

         
        gf2_matrix_square(odd, even);
        if (len2 & 1)
            crc1 = gf2_matrix_times(odd, crc1);
        len2 >>= 1;

         
    } while (len2 != 0);

     
    crc1 ^= crc2;
    return crc1;
}
