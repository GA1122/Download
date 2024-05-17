ft_outline_new_circle( FT_Outline*     outline,
                       FT_F26Dot6      radius,
                       FTDemo_Handle*  handle )
{
  char*       tag;
  FT_Vector*  vec;
  FT_F26Dot6  disp = (FT_F26Dot6)( radius * 0.6781 );

  FT_Outline_New( handle->library, 12, 1, outline );
  outline->n_points   = 12;
  outline->n_contours = 1;
  outline->contours[0] = outline->n_points-1;

  vec = outline->points;
  tag = outline->tags;

  vec->x =  radius; vec->y =       0; vec++; *tag++ = FT_CURVE_TAG_ON;
  vec->x =  radius; vec->y =    disp; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
  vec->x =    disp; vec->y =  radius; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
  vec->x =       0; vec->y =  radius; vec++; *tag++ = FT_CURVE_TAG_ON;
  vec->x =   -disp; vec->y =  radius; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
  vec->x = -radius; vec->y =    disp; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
  vec->x = -radius; vec->y =       0; vec++; *tag++ = FT_CURVE_TAG_ON;
  vec->x = -radius; vec->y =   -disp; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
  vec->x =   -disp; vec->y = -radius; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
  vec->x =       0; vec->y = -radius; vec++; *tag++ = FT_CURVE_TAG_ON;
  vec->x =    disp; vec->y = -radius; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
  vec->x =  radius; vec->y =   -disp; vec++; *tag++ = FT_CURVE_TAG_CUBIC;
}
