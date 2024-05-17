grid_status_draw_outline( GridStatus       st,
                          FTDemo_Handle*   handle,
                          FTDemo_Display*  display )
{
  static FT_Stroker  stroker;
  FT_Size            size;
  FT_GlyphSlot       slot;
  double             scale = 64.0 * st->scale;
  int                ox    = (int)st->x_origin;
  int                oy    = (int)st->y_origin;


  if ( stroker == NULL )
  {
    FT_Stroker_New( handle->library, &stroker );

    FT_Stroker_Set( stroker, 32, FT_STROKER_LINECAP_BUTT,
                    FT_STROKER_LINEJOIN_ROUND, 0x20000 );
  }

  FTDemo_Get_Size( handle, &size );

  _af_debug_disable_horz_hints = !st->do_horz_hints;
  _af_debug_disable_vert_hints = !st->do_vert_hints;

  if ( FT_Load_Glyph( size->face, st->Num,
                      handle->load_flags | FT_LOAD_NO_BITMAP ) )
    return;

  slot = size->face->glyph;
  if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
  {
    FT_Glyph     glyph;
    FT_Outline*  gimage = &slot->outline;
    int          nn;


     
    for (nn = 0; nn < gimage->n_points; nn++)
    {
      FT_Vector*  vec = &gimage->points[nn];


      vec->x = (FT_F26Dot6)( vec->x * scale );
      vec->y = (FT_F26Dot6)( vec->y * scale );
    }

     
    if ( st->do_outline )
    {
      FT_Get_Glyph( slot, &glyph );
      FT_Glyph_Stroke( &glyph, stroker, 1 );

      FTDemo_Draw_Glyph_Color( handle, display, glyph, &ox, &oy,
                               st->outline_color );
      FT_Done_Glyph( glyph );
    }

     
    if ( st->do_dots )
    {
      for (nn = 0; nn < gimage->n_points; nn++)
        circle_draw( (FT_F26Dot6)( st->x_origin * 64 + gimage->points[nn].x ),
                     (FT_F26Dot6)( st->y_origin * 64 - gimage->points[nn].y ),
                     128,
                     handle,
                     display,
                     ( gimage->tags[nn] & FT_CURVE_TAG_ON )
                       ? st->on_color
                       : st->conic_color );
    }
  }
}
