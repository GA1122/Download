__imlib_RenderImageSkewed(Display * d, ImlibImage * im, Drawable w, Drawable m,
                          Visual * v, Colormap cm, int depth,
                          int sx, int sy, int sw, int sh, int dx, int dy,
                          int hsx, int hsy, int vsx, int vsy,
                          char antialias, char hiq, char blend,
                          char dither_mask, int mat, ImlibColorModifier * cmod,
                          ImlibOp op)
{
   int                 dx1, dy1, dx2, dy2, dw, dh, tsx, tsy;
   ImlibImage         *back;

   dx1 = dx2 = dx;
   dy1 = dy2 = dy;

   if (hsx < 0)
      dx1 += hsx;
   else
      dx2 += hsx;
   if (hsy < 0)
      dy1 += hsy;
   else
      dy2 += hsy;
   tsx = vsx;
   tsy = vsy;
   if (!tsx && !tsy)
     {
        tsy = (hsx * im->h) / im->w;
        tsx = -(hsy * im->h) / im->w;
     }
   if (tsx < 0)
      dx1 += tsx;
   else
      dx2 += tsx;
   if (tsy < 0)
      dy1 += tsy;
   else
      dy2 += tsy;

   if ((dx2 < 0) || (dy2 < 0))
      return;

   dw = dx2 - dx1;
   dh = dy2 - dy1;

   if ((dw <= 0) || (dh <= 0))
      return;

   if (dx1 < 0)
     {
        dw += dx1;
        dx1 = 0;
     }
   if (dy1 < 0)
     {
        dh += dy1;
        dy1 = 0;
     }

   __imlib_GetContext(d, v, cm, depth);

   back = __imlib_CreateImage(dw, dh, NULL);
   back->data = calloc(dw * dh, sizeof(DATA32));
   __imlib_GrabDrawableToRGBA(back->data, 0, 0, dw, dh, d, w, 0, v, cm,
                              depth, dx1, dy1, dw, dh, 0, 1);

   __imlib_BlendImageToImageSkewed(im, back, antialias, 1, 0, sx, sy, sw, sh,
                                   dx - dx1, dy - dy1, hsx, hsy, vsx, vsy,
                                   cmod, op, 0, 0, 0, 0);

   __imlib_RenderImage(d, back, w, m, v, cm, depth, 0, 0, dw, dh,
                       dx1, dy1, dw, dh, 0, hiq, 0, dither_mask, mat, 0,
                       OP_COPY);
   __imlib_FreeImage(back);
}
