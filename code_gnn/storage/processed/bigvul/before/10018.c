ft_outline_draw( FT_Outline*      outline,
                 double           scale,
                 int              pen_x,
                 int              pen_y,
                 FTDemo_Handle*   handle,
                 FTDemo_Display*  display,
                 grColor          color )
{
  FT_Outline  transformed;
  FT_BBox     cbox;
  FT_Bitmap   bitm;

  FT_Outline_New( handle->library,
                  outline->n_points,
                  outline->n_contours,
                  &transformed );

  FT_Outline_Copy( outline, &transformed );

  if ( scale != 1. )
  {
    int  nn;

    for ( nn = 0; nn < transformed.n_points; nn++ )
    {
      FT_Vector*  vec = &transformed.points[nn];

      vec->x = (FT_F26Dot6)(vec->x*scale);
      vec->y = (FT_F26Dot6)(vec->y*scale);
    }
  }

  FT_Outline_Get_CBox( &transformed, &cbox );
  cbox.xMin &= ~63;
  cbox.yMin &= ~63;
  cbox.xMax  = (cbox.xMax + 63) & ~63;
  cbox.yMax  = (cbox.yMax + 63) & ~63;

  bitm.width      = (cbox.xMax - cbox.xMin) >> 6;
  bitm.rows       = (cbox.yMax - cbox.yMin) >> 6;
  bitm.pitch      = bitm.width;
  bitm.num_grays  = 256;
  bitm.pixel_mode = FT_PIXEL_MODE_GRAY;
  bitm.buffer     = (unsigned char*)calloc( bitm.pitch, bitm.rows );

  FT_Outline_Translate( &transformed, -cbox.xMin, -cbox.yMin );
  FT_Outline_Get_Bitmap( handle->library, &transformed, &bitm );

  ft_bitmap_draw( &bitm,
                  pen_x + (cbox.xMin >> 6),
                  pen_y - (cbox.yMax >> 6),
                  display,
                  color );

  free( bitm.buffer );
  FT_Outline_Done( handle->library, &transformed );
}
