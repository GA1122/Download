static void ipa_device_begin(wmfAPI * API)
{
  char
    comment[MaxTextExtent];

  wmf_magick_t
    *ddata = WMF_MAGICK_GetData(API);

   
  (void) PushDrawingWand(WmfDrawingWand);

  DrawSetViewbox(WmfDrawingWand, 0, 0, ddata->image->columns, ddata->image->rows );

  (void) FormatLocaleString(comment,MaxTextExtent,"Created by ImageMagick %s",
    GetMagickVersion((size_t *) NULL));
  DrawComment(WmfDrawingWand,comment);

   
  DrawScale(WmfDrawingWand, ddata->scale_x, ddata->scale_y);

   
  DrawTranslate(WmfDrawingWand, ddata->translate_x, ddata->translate_y);

   
  DrawRotate(WmfDrawingWand, ddata->rotate);

  if (ddata->image_info->texture == NULL)
    {
      PixelWand
        *background_color;

       
      background_color=NewPixelWand();
      PixelSetQuantumColor(background_color,&ddata->image->background_color);
      DrawSetFillColor(WmfDrawingWand,background_color);
      background_color=DestroyPixelWand(background_color);
      DrawRectangle(WmfDrawingWand,
                     XC(ddata->bbox.TL.x),YC(ddata->bbox.TL.y),
                     XC(ddata->bbox.BR.x),YC(ddata->bbox.BR.y));
    }
  else
    {
       
      Image
        *image;

      ImageInfo
        *image_info;

      ExceptionInfo
        *exception;

      exception=AcquireExceptionInfo();

      image_info = CloneImageInfo((ImageInfo *) 0);
      (void) CopyMagickString(image_info->filename,ddata->image_info->texture,
        MaxTextExtent);
      if ( ddata->image_info->size )
        CloneString(&image_info->size,ddata->image_info->size);

      image = ReadImage(image_info,exception);
      image_info=DestroyImageInfo(image_info);
      if (image)
        {
          char
            pattern_id[30];

          MagickWand
            *magick_wand;

          (void) CopyMagickString(image->magick,"MIFF",MaxTextExtent);
          DrawPushDefs(WmfDrawingWand);
          draw_pattern_push(API,ddata->pattern_id,image->columns,image->rows);
          magick_wand=NewMagickWandFromImage(image);
          (void) DrawComposite(WmfDrawingWand,CopyCompositeOp,0,0,
            image->columns,image->rows,magick_wand);
          magick_wand=DestroyMagickWand(magick_wand);
          (void) DrawPopPattern(WmfDrawingWand);
          DrawPopDefs(WmfDrawingWand);
          (void) FormatLocaleString(pattern_id,MaxTextExtent,"#brush_%lu",
            ddata->pattern_id);
          (void) DrawSetFillPatternURL(WmfDrawingWand,pattern_id);
          ++ddata->pattern_id;

          DrawRectangle(WmfDrawingWand,
                         XC(ddata->bbox.TL.x),YC(ddata->bbox.TL.y),
                         XC(ddata->bbox.BR.x),YC(ddata->bbox.BR.y));
          image=DestroyImageList(image);
        }
      else
        {
          LogMagickEvent(CoderEvent,GetMagickModule(),
            "reading texture image failed!");
          InheritException(&ddata->image->exception,exception);
        }
      (void) DestroyExceptionInfo(exception);
    }

  DrawSetClipRule(WmfDrawingWand,EvenOddRule);  
  draw_fill_color_string(WmfDrawingWand,"none");  
  draw_stroke_color_string(WmfDrawingWand,"none");  
  DrawSetStrokeLineCap(WmfDrawingWand,ButtCap);  
  DrawSetStrokeLineJoin(WmfDrawingWand,MiterJoin);  
  draw_under_color_string(WmfDrawingWand,"white");  
}
