static void ipa_draw_text(wmfAPI * API, wmfDrawText_t * draw_text)
{
  double
    angle = 0,       
    bbox_height,     
    bbox_width,       
    pointsize = 0;     

  TypeMetric
    metrics;

  wmfD_Coord
    BL,         
    BR,         
    TL,         
    TR;         

  wmfD_Coord
    point;       

  wmfFont
    *font;

  wmf_magick_t
    * ddata = WMF_MAGICK_GetData(API);

  point = draw_text->pt;

   
  {
    double dx,
      dy;

    if ( draw_text->flags)
      {
        TL = draw_text->TL;
        BR = draw_text->BR;
        TR.x = draw_text->BR.x;
        TR.y = draw_text->TL.y;
        BL.x = draw_text->TL.x;
        BL.y = draw_text->BR.y;
      }
    else
      {
        TL = draw_text->bbox.TL;
        BR = draw_text->bbox.BR;
        TR = draw_text->bbox.TR;
        BL = draw_text->bbox.BL;
      }
    dx = ((TR.x - TL.x) + (BR.x - BL.x)) / 2;
    dy = ((TR.y - TL.y) + (BR.y - BL.y)) / 2;
    bbox_width = hypot(dx,dy);
    dx = ((BL.x - TL.x) + (BR.x - TR.x)) / 2;
    dy = ((BL.y - TL.y) + (BR.y - TR.y)) / 2;
    bbox_height = hypot(dx,dy);
  }

  font = WMF_DC_FONT(draw_text->dc);

   
  pointsize = util_pointsize( API, font, draw_text->str, draw_text->font_height);

   
  (void) PushDrawingWand(WmfDrawingWand);

  (void) bbox_width;
  (void) bbox_height;
#if 0
  printf("\nipa_draw_text\n");
  printf("Text                    = \"%s\"\n", draw_text->str);
   
  printf("WMF_FONT_PSNAME:        = \"%s\"\n", WMF_FONT_PSNAME(font));
  printf("Bounding box            TL=%g,%g BR=%g,%g\n",
         TL.x, TL.y, BR.x, BR.y );
   
   
  printf("Pointsize               = %g\n", pointsize);
  fflush(stdout);
#endif

   
  if ((WMF_DC_TEXTALIGN(draw_text->dc) & TA_CENTER) ||
      (WMF_TEXT_UNDERLINE(font)) || (WMF_TEXT_STRIKEOUT(font)))
    {
      Image
        *image = ddata->image;

      DrawInfo
        *draw_info;

      draw_info=ddata->draw_info;
      draw_info->font=WMF_FONT_PSNAME(font);
      draw_info->pointsize = pointsize;
      draw_info->text=draw_text->str;

      if (GetTypeMetrics(image, draw_info, &metrics) != MagickFalse)
        {
           
          if ((WMF_DC_TEXTALIGN(draw_text->dc) & TA_CENTER))
            {
              double
                text_width = metrics.width * (ddata->scale_y / ddata->scale_x);

#if defined(MAGICKCORE_WMFLITE_DELEGATE)
              point.x -= text_width / 2;
#else
              point.x += bbox_width / 2 - text_width / 2;
#endif
            }
        }
      draw_info->font=NULL;
      draw_info->text=NULL;
    }

   
  if (draw_text->flags & ETO_OPAQUE)
    {
       
      draw_stroke_color_string(WmfDrawingWand,"none");
      draw_fill_color_rgb(API,WMF_DC_BACKGROUND(draw_text->dc));
      DrawRectangle(WmfDrawingWand,
                    XC(draw_text->TL.x),YC(draw_text->TL.y),
                    XC(draw_text->BR.x),YC(draw_text->BR.y));
      draw_fill_color_string(WmfDrawingWand,"none");
    }
  else
    {
       
      if (WMF_DC_OPAQUE(draw_text->dc))
        {
          wmfRGB
            *box = WMF_DC_BACKGROUND(draw_text->dc);

          PixelWand
            *under_color;

          under_color=NewPixelWand();
          PixelSetRedQuantum(under_color,ScaleCharToQuantum(box->r));
          PixelSetGreenQuantum(under_color,ScaleCharToQuantum(box->g));
          PixelSetBlueQuantum(under_color,ScaleCharToQuantum(box->b));
          PixelSetOpacityQuantum(under_color,OpaqueOpacity);
          DrawSetTextUnderColor(WmfDrawingWand,under_color);
          under_color=DestroyPixelWand(under_color);
        }
      else
        draw_under_color_string(WmfDrawingWand,"none");
    }

   
  if ( draw_text->flags & ETO_CLIPPED)
    {
    }

   
  draw_stroke_color_string(WmfDrawingWand,"none");

   
  draw_fill_color_rgb(API,WMF_DC_TEXTCOLOR(draw_text->dc));

   
  (void) DrawSetFontSize(WmfDrawingWand,pointsize);

   
  (void) DrawSetFont(WmfDrawingWand, WMF_FONT_PSNAME(font));

   
  DrawTranslate(WmfDrawingWand, XC(point.x), YC(point.y));

   
  DrawScale(WmfDrawingWand, ddata->scale_y / ddata->scale_x, 1.0);

   
   
  angle = fabs(RadiansToDegrees(2 * MagickPI - WMF_TEXT_ANGLE(font)));
  if (angle == 360)
    angle = 0;
  if (angle != 0)
    DrawRotate(WmfDrawingWand, angle);

   

   
  DrawAnnotation(WmfDrawingWand, 0, 0, (unsigned char*)draw_text->str);

   
  if (WMF_TEXT_UNDERLINE(font))
    {
      double
        line_height;

      wmfD_Coord
        ulBR,       
        ulTL;       

      line_height = ((double)1/(ddata->scale_x))*metrics.underline_thickness;
      if (metrics.underline_thickness < 1.5)
        line_height *= 0.55;
      ulTL.x = 0;
      ulTL.y = fabs(metrics.descent) - line_height;
      ulBR.x = metrics.width;
      ulBR.y = fabs(metrics.descent);

      DrawRectangle(WmfDrawingWand,
                    XC(ulTL.x), YC(ulTL.y), XC(ulBR.x), YC(ulBR.y));
    }

   
  if (WMF_TEXT_STRIKEOUT(font))
    {
      double line_height;

      wmfD_Coord
        ulBR,       
        ulTL;       

      line_height = ((double)1/(ddata->scale_x))*metrics.underline_thickness;

      if (metrics.underline_thickness < 2.0)
        line_height *= 0.55;
      ulTL.x = 0;
      ulTL.y = -(((double) metrics.ascent) / 2 + line_height / 2);
      ulBR.x = metrics.width;
      ulBR.y = -(((double) metrics.ascent) / 2 - line_height / 2);

      DrawRectangle(WmfDrawingWand,
                    XC(ulTL.x), YC(ulTL.y), XC(ulBR.x), YC(ulBR.y));

    }

   
  (void) PopDrawingWand(WmfDrawingWand);

#if 0
  (void) PushDrawingWand(WmfDrawingWand);
  draw_stroke_color_string(WmfDrawingWand,"red");
  draw_fill_color_string(WmfDrawingWand,"none");
  DrawRectangle(WmfDrawingWand,
                XC(TL.x), YC(TL.y),
                XC(BR.x), YC(BR.y));
  draw_stroke_color_string(WmfDrawingWand,"none");
  (void) PopDrawingWand(WmfDrawingWand);
#endif

}
