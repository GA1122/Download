__imlib_generic_render(DATA32 * src, int jump, int w, int h, int dx, int dy,
                       XImage * xim, Visual * v, Context * ct)
{
   int                 x, y, hh;
   unsigned int        r, g, b, val;
   unsigned int        rmask, gmask, bmask;
   int                 i, rshift, gshift, bshift;

   static const DATA8  _dither_88[8][8] = {
      {0, 32, 8, 40, 2, 34, 10, 42},
      {48, 16, 56, 24, 50, 18, 58, 26},
      {12, 44, 4, 36, 14, 46, 6, 38},
      {60, 28, 52, 20, 62, 30, 54, 22},
      {3, 35, 11, 43, 1, 33, 9, 41},
      {51, 19, 59, 27, 49, 17, 57, 25},
      {15, 47, 7, 39, 13, 45, 5, 37},
      {63, 31, 55, 23, 61, 29, 53, 21}
   };

   if (xim->depth == 1)
     {
        hh = dy + h;
        for (y = dy; y < hh; y++)
          {
             for (x = dx; x < w; x++)
               {
                  r = R_VAL(src);
                  g = G_VAL(src);
                  b = B_VAL(src);
                  val = (R_VAL(src) + G_VAL(src) + B_VAL(src)) / 12;
                  if (val > _dither_88[x & 0x3][y & 0x3])
                     val = ct->palette[1];
                  else
                     val = ct->palette[0];
                  XPutPixel(xim, x, y, val);
                  src++;
               }
          }
        return;
     }
   rmask = xim->red_mask;
   gmask = xim->green_mask;
   bmask = xim->blue_mask;
   rshift = 0;
   gshift = 0;
   bshift = 0;
   for (i = 31; i >= 0; i--)
     {
        if (rmask >= (1U << i))
          {
             rshift = i - 7;
             break;
          }
     }
   for (i = 31; i >= 0; i--)
     {
        if (gmask >= (1U << i))
          {
             gshift = i - 7;
             break;
          }
     }
   for (i = 31; i >= 0; i--)
     {
        if (bmask >= (1U << i))
          {
             bshift = i - 7;
             break;
          }
     }
   hh = dy + h;
   for (y = dy; y < hh; y++)
     {
        for (x = dx; x < w; x++)
          {
             r = R_VAL(src);
             if (rshift >= 0)
                val = ((r << rshift) & rmask);
             else
                val = ((r >> (-rshift)) & rmask);
             g = G_VAL(src);
             if (gshift >= 0)
                val |= ((g << gshift) & gmask);
             else
                val |= ((g >> (-gshift)) & gmask);
             b = B_VAL(src);
             if (bshift >= 0)
                val |= ((b << bshift) & bmask);
             else
                val |= ((b >> (-bshift)) & bmask);
             XPutPixel(xim, x, y, val);
             src++;
          }
     }
}
