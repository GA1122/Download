  mm_axis_unmap( PS_DesignMap  axismap,
                 FT_Fixed      ncv )
  {
    int  j;


    if ( ncv <= axismap->blend_points[0] )
      return INT_TO_FIXED( axismap->design_points[0] );

    for ( j = 1; j < axismap->num_points; ++j )
    {
      if ( ncv <= axismap->blend_points[j] )
        return INT_TO_FIXED( axismap->design_points[j - 1] ) +
               ( axismap->design_points[j] - axismap->design_points[j - 1] ) *
               FT_DivFix( ncv - axismap->blend_points[j - 1],
                          axismap->blend_points[j] -
                            axismap->blend_points[j - 1] );
    }

    return INT_TO_FIXED( axismap->design_points[axismap->num_points - 1] );
  }
