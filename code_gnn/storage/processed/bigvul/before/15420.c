__imlib_RenderImage(Display * d, ImlibImage * im,
                    Drawable w, Drawable m,
                    Visual * v, Colormap cm, int depth,
                    int sx, int sy, int sw, int sh,
                    int dx, int dy, int dw, int dh,
                    char antialias, char hiq, char blend, char dither_mask,
                    int mat, ImlibColorModifier * cmod, ImlibOp op)
{
   XImage             *xim = NULL, *mxim = NULL;
   Context            *ct;
   DATA32             *buf = NULL, *pointer = NULL, *back = NULL;
   int                 y, h, hh, jump;
   XGCValues           gcv;
   ImlibScaleInfo     *scaleinfo = NULL;
   int                 psx, psy, psw, psh;
   char                shm = 0;
   ImlibRGBAFunction   rgbaer;
   ImlibMaskFunction   masker = NULL;
   ImlibBlendFunction  blender = NULL;

#ifdef DO_MMX_ASM
   int                 do_mmx;
#endif

   blender = __imlib_GetBlendFunction(op, 1, 0,
                                      (!(im->flags & F_HAS_ALPHA)), NULL);

    
   if ((dw == 0) || (dh == 0))
      return;
    
   if ((sw <= 0) || (sh <= 0))
      return;
    
   if ((abs(dw) > X_MAX_DIM) || (abs(dh) > X_MAX_DIM))
      return;
    
   psx = sx;
   psy = sy;
   psw = sw;
   psh = sh;
   CLIP(sx, sy, sw, sh, 0, 0, im->w, im->h);
    
   if (psx != sx)
      dx = (dx * sx) / psx;
   if (psy != sy)
      dy = (dy * sy) / psy;
   if (psw != sw)
      dw = (dw * sw) / psw;
   if (psh != sh)
      dh = (dh * sh) / psh;
    
    
   if ((dw == 0) || (dh == 0))
      return;
    
   if ((sw <= 0) || (sh <= 0))
      return;
    
   if ((abs(dw) > X_MAX_DIM) || (abs(dh) > X_MAX_DIM))
      return;
    
   if (!((sw == dw) && (sh == dh)))
     {
        scaleinfo = __imlib_CalcScaleInfo(im, sw, sh, dw, dh, antialias);
        if (!scaleinfo)
           return;
     }
    
   dw = abs(dw);
   dh = abs(dh);
   ct = __imlib_GetContext(d, v, cm, depth);
   __imlib_RGBASetupContext(ct);
   if ((blend) && (IMAGE_HAS_ALPHA(im)))
     {
        back = malloc(dw * dh * sizeof(DATA32));
        if (!__imlib_GrabDrawableToRGBA
            (back, 0, 0, dw, dh, d, w, 0, v, cm, depth, dx, dy, dw, dh, 0, 1))
          {
             free(back);
             back = NULL;
          }
     }
    
   xim = __imlib_ProduceXImage(d, v, depth, dw, dh, &shm);
   if (!xim)
     {
        __imlib_FreeScaleInfo(scaleinfo);
        if (back)
           free(back);
        return;
     }
   if (m)
     {
        mxim = __imlib_ProduceXImage(d, v, 1, dw, dh, &shm);
        if (!mxim)
          {
             __imlib_ConsumeXImage(d, xim);
             __imlib_FreeScaleInfo(scaleinfo);
             if (back)
                free(back);
             return;
          }
        memset(mxim->data, 0, mxim->bytes_per_line * mxim->height);
     }
    
   if (scaleinfo)
     {
         
        buf = malloc(dw * LINESIZE * sizeof(DATA32));
        if (!buf)
          {
             __imlib_ConsumeXImage(d, xim);
             if (m)
                __imlib_ConsumeXImage(d, mxim);
             __imlib_FreeScaleInfo(scaleinfo);
             if (back)
                free(back);
             return;
          }
     }
    
   h = dh;
    
    
   rgbaer = __imlib_GetRGBAFunction(xim->bits_per_pixel,
                                    v->red_mask, v->green_mask, v->blue_mask,
                                    hiq, ct->palette_type);
   if (m)
      masker = __imlib_GetMaskFunction(dither_mask);
#ifdef DO_MMX_ASM
   do_mmx = __imlib_get_cpuid() & CPUID_MMX;
#endif
   for (y = 0; y < dh; y += LINESIZE)
     {
        hh = LINESIZE;
        if (h < LINESIZE)
           hh = h;
         
        if (scaleinfo)
          {
              
             if (antialias)
               {
#ifdef DO_MMX_ASM
                  if (do_mmx)
                     __imlib_Scale_mmx_AARGBA(scaleinfo, buf,
                                              ((sx * dw) / sw),
                                              ((sy * dh) / sh) + y,
                                              0, 0, dw, hh, dw, im->w);
                  else
#endif
                  if (IMAGE_HAS_ALPHA(im))
                     __imlib_ScaleAARGBA(scaleinfo, buf, ((sx * dw) / sw),
                                         ((sy * dh) / sh) + y,
                                         0, 0, dw, hh, dw, im->w);
                  else
                     __imlib_ScaleAARGB(scaleinfo, buf, ((sx * dw) / sw),
                                        ((sy * dh) / sh) + y,
                                        0, 0, dw, hh, dw, im->w);
               }
             else
                __imlib_ScaleSampleRGBA(scaleinfo, buf, ((sx * dw) / sw),
                                        ((sy * dh) / sh) + y, 0, 0, dw, hh, dw);
             jump = 0;
             pointer = buf;
             if (cmod)
                __imlib_DataCmodApply(buf, dw, hh, 0, NULL, cmod);
          }
        else
          {
             if (cmod)
               {
                  if (!buf)
                     buf = malloc(im->w * LINESIZE * sizeof(DATA32));
                  if (!buf)
                    {
                       __imlib_ConsumeXImage(d, xim);
                       if (m)
                          __imlib_ConsumeXImage(d, mxim);
                       __imlib_FreeScaleInfo(scaleinfo);
                       if (back)
                          free(back);
                       return;
                    }
                  memcpy(buf, im->data + ((y + sy) * im->w),
                         im->w * hh * sizeof(DATA32));
                  __imlib_DataCmodApply(buf, im->w, hh, 0, NULL, cmod);
                  pointer = buf + sx;
                  jump = im->w - sw;
               }
             else
               {
                  jump = im->w - sw;
                  pointer = im->data + ((y + sy) * im->w) + sx;
               }
          }
         
        if (back)
          {
             blender(pointer, jump + dw, back + (y * dw), dw, dw, hh, NULL);
             pointer = back + (y * dw);
             jump = 0;
          }
         
        if (rgbaer)
           rgbaer(pointer, jump,
                  ((DATA8 *) xim->data) + (y * (xim->bytes_per_line)),
                  xim->bytes_per_line, dw, hh, dx, dy + y);
        else
           __imlib_generic_render(pointer, jump, dw, hh, 0, y, xim, v, ct);
        if (m)
           masker(pointer, jump,
                  ((DATA8 *) mxim->data) + (y * (mxim->bytes_per_line)),
                  mxim->bytes_per_line, dw, hh, dx, dy + y, mat);
        h -= LINESIZE;
     }
    
   if (buf)
      free(buf);
   if (scaleinfo)
      __imlib_FreeScaleInfo(scaleinfo);
   if (back)
      free(back);
    
   if ((gc) && ((last_depth != depth) || (disp != d)))
     {
        XFreeGC(disp, gc);
        gc = 0;
     }
    
   if (!gc)
     {
        disp = d;
        last_depth = depth;
        gcv.graphics_exposures = False;
        gc = XCreateGC(d, w, GCGraphicsExposures, &gcv);
     }
   if (m)
     {
         
        if ((gcm) && (disp != d))
          {
             XFreeGC(disp, gcm);
             gcm = 0;
          }
        if (!gcm)
          {
             gcv.graphics_exposures = False;
             gcm = XCreateGC(d, m, GCGraphicsExposures, &gcv);
          }
         
        if (shm)
            
           XShmPutImage(d, m, gcm, mxim, 0, 0, dx, dy, dw, dh, False);
         
        else
           XPutImage(d, m, gcm, mxim, 0, 0, dx, dy, dw, dh);
     }
    
   if (shm)
       
      XShmPutImage(d, w, gc, xim, 0, 0, dx, dy, dw, dh, False);
    
   else
      XPutImage(d, w, gc, xim, 0, 0, dx, dy, dw, dh);
    
    
   if (shm)
      XSync(d, False);
   __imlib_ConsumeXImage(d, xim);
   if (m)
      __imlib_ConsumeXImage(d, mxim);
}
