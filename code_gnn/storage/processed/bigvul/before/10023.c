grid_status_rescale_initial( GridStatus      st,
                             FTDemo_Handle*  handle )
{
  FT_Size   size;
  FT_Error  err = FTDemo_Get_Size( handle, &size );


  if ( !err )
  {
    FT_Face  face = size->face;

    int  xmin = FT_MulFix( face->bbox.xMin, size->metrics.x_scale );
    int  ymin = FT_MulFix( face->bbox.yMin, size->metrics.y_scale );
    int  xmax = FT_MulFix( face->bbox.xMax, size->metrics.x_scale );
    int  ymax = FT_MulFix( face->bbox.yMax, size->metrics.y_scale );

    double  x_scale, y_scale;


    xmin &= ~63;
    ymin &= ~63;
    xmax  = ( xmax + 63 ) & ~63;
    ymax  = ( ymax + 63 ) & ~63;

    printf( "XXX x_ppem=%d y_ppem=%d width=%d height=%d\n",
            size->metrics.x_ppem, size->metrics.y_ppem,
            xmax - xmin, ymax - ymin );

    x_scale = st->disp_width  * ( 1.0 - 2 * st->margin ) / ( xmax - xmin );
    y_scale = st->disp_height * ( 1.0 - 2 * st->margin ) / ( ymax - ymin );

    if ( x_scale <= y_scale )
      st->scale = x_scale;
    else
      st->scale = y_scale;

    st->x_origin = st->disp_width  * st->margin         - xmin * st->scale;
    st->y_origin = st->disp_height * ( 1 - st->margin ) + ymin * st->scale;
  }
  else
  {
    st->scale    = 1.;
    st->x_origin = st->disp_width  * st->margin;
    st->y_origin = st->disp_height * st->margin;
  }

  st->scale_0    = st->scale;
  st->x_origin_0 = st->x_origin;
  st->y_origin_0 = st->y_origin;
}
