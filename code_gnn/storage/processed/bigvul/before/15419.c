__imlib_RenderGetPixel(Display * d, Drawable w, Visual * v, Colormap cm,
                       int depth, DATA8 r, DATA8 g, DATA8 b)
{
   Context            *ct;

   ct = __imlib_GetContext(d, v, cm, depth);

   if (ct->palette)
     {
        switch (ct->palette_type)
          {
          case 0:               
             return ct->palette[((r >> 0) & 0xe0) |
                                ((g >> 3) & 0x1b) | ((b >> 6) & 0x02)];
             break;
          case 1:               
             return ct->palette[((r >> 0) & 0xe0) |
                                ((g >> 3) & 0x1b) | ((b >> 6) & 0x02)];
             break;
          case 2:               
             return ct->palette[((r >> 0) & 0xe0) |
                                ((g >> 3) & 0x1b) | ((b >> 6) & 0x02)];
             break;
          case 3:               
             return ct->palette[((r >> 0) & 0xe0) |
                                ((g >> 3) & 0x1b) | ((b >> 6) & 0x02)];
             break;
          case 4:               
             return ct->palette[((r >> 0) & 0xe0) |
                                ((g >> 3) & 0x1b) | ((b >> 6) & 0x02)];
             break;
          case 5:               
             return ct->palette[((r >> 0) & 0xe0) |
                                ((g >> 3) & 0x1b) | ((b >> 6) & 0x02)];
             break;
          case 6:               
             return ct->palette[((r >> 0) & 0xe0) |
                                ((g >> 3) & 0x1b) | ((b >> 6) & 0x02)];
             break;
          case 7:               
             return ct->palette[((int)(((double)r / 255) * 5.0) * 36) +
                                ((int)(((double)g / 255) * 5.0) * 6) +
                                ((int)(((double)b / 255) * 5.0))];
             break;
          default:
             return 0;
          }
     }
   else
     {
        unsigned int        rm, gm, bm;
        int                 i, rshift = 0, gshift = 0, bshift = 0;
        DATA32              val;

        rm = v->red_mask;
        gm = v->green_mask;
        bm = v->blue_mask;
        if ((rm == 0xf800) && (gm == 0x7e0) && (bm == 0x1f))     
          {
             return (((r << 8) & 0xf800) |
                     ((g << 3) & 0x07e0) | ((b >> 3) & 0x001f));
          }
        if ((rm == 0xff0000) && (gm == 0xff00) && (bm == 0xff))  
          {
             return (((r << 16) & 0xff0000) |
                     ((g << 8) & 0x00ff00) | ((r) & 0x0000ff));
          }
        if ((rm == 0x7c00) && (gm == 0x3e0) && (bm == 0x1f))     
          {
             return (((r << 7) & 0x7c00) |
                     ((g << 2) & 0x03e0) | ((b >> 3) & 0x001f));
          }
        for (i = 31; i >= 0; i--)
          {
             if (rm >= (1U << i))
               {
                  rshift = i - 7;
                  break;
               }
          }
        for (i = 31; i >= 0; i--)
          {
             if (gm >= (1U << i))
               {
                  gshift = i - 7;
                  break;
               }
          }
        for (i = 31; i >= 0; i--)
          {
             if (bm >= (1U << i))
               {
                  bshift = i - 7;
                  break;
               }
          }
        if (rshift >= 0)
           val = ((r << rshift) & rm);
        else
           val = ((r >> (-rshift)) & rm);
        if (gshift >= 0)
           val |= ((g << gshift) & gm);
        else
           val |= ((g >> (-gshift)) & gm);
        if (bshift >= 0)
           val |= ((b << bshift) & bm);
        else
           val |= ((b >> (-bshift)) & bm);
        return val;
     }
   return 0;
}
