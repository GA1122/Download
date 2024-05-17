pdf14_copy_mono(gx_device * dev,
               const byte * base, int sourcex, int sraster, gx_bitmap_id id,
        int x, int y, int w, int h, gx_color_index zero, gx_color_index one)
{
    const byte *sptr;
    const byte *line;
    int sbit, first_bit;
    int code, sbyte, bit, count;
    int run_length, startx, current_bit, bit_value;
    gx_color_index current_color;

    fit_copy(dev, base, sourcex, sraster, id, x, y, w, h);
    line = base + (sourcex >> 3);
    sbit = sourcex & 7;
    first_bit = 7 - sbit;

     
    while (h-- > 0) {
         
        sptr = line;
        sbyte = *sptr++;
        bit = first_bit;
        count = w;
        run_length = 0;
        startx = x;
        current_bit = 0;
        current_color = zero;

         
        do {
            bit_value = (sbyte >> bit) & 1;
            if (bit_value == current_bit) {
                 
                run_length++;
            } else {
                 
                if (run_length != 0) {
                    if (current_color != gx_no_color_index) {
                        code = (*dev_proc(dev, fill_rectangle))
                                (dev, startx, y, run_length, 1, current_color);
                        if (code < 0)
                            return code;
                    }
                    startx += run_length;
                }
                run_length = 1;
                current_color = bit_value ? one : zero;
                current_bit = bit_value;
            }
             
            if (bit == 0) {
                bit = 7;
                sbyte = *sptr++;
            }
            else
                bit--;
        } while (--count > 0);
         
        if (run_length != 0 && current_color != gx_no_color_index) {
            code = (*dev_proc(dev, fill_rectangle))
                        (dev, startx, y, run_length, 1, current_color);
            if (code < 0)
                return code;
        }
         
        line += sraster;
        y++;
    }
    return 0;
}
