png_read_filter_row_paeth_1byte_pixel(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row)
{
   png_bytep rp_end = row + row_info->rowbytes;
   int a, c;

    
   c = *prev_row++;
   a = *row + c;
   *row++ = (png_byte)a;

    
   while (row < rp_end)
   {
      int b, pa, pb, pc, p;

      a &= 0xff;  
      b = *prev_row++;

      p = b - c;
      pc = a - c;

#ifdef PNG_USE_ABS
      pa = abs(p);
      pb = abs(pc);
      pc = abs(p + pc);
#else
      pa = p < 0 ? -p : p;
      pb = pc < 0 ? -pc : pc;
      pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#endif

       
      if (pb < pa)
      {
         pa = pb; a = b;
      }
      if (pc < pa) a = c;

       
      c = b;
      a += *row;
      *row++ = (png_byte)a;
   }
}
