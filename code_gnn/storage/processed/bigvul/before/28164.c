static void put_pixels_clamped2_c(const int16_t *block, uint8_t *av_restrict pixels,
                                 int line_size)
{
    int i;

     
    for(i=0;i<2;i++) {
        pixels[0] = av_clip_uint8(block[0]);
        pixels[1] = av_clip_uint8(block[1]);

        pixels += line_size;
        block += 8;
    }
}