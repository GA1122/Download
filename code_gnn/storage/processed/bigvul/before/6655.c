  T1_Set_MM_Design( T1_Face   face,
                    FT_UInt   num_coords,
                    FT_Long*  coords )
  {
    PS_Blend  blend = face->blend;
    FT_Error  error;
    FT_UInt   n, p;


    error = FT_ERR( Invalid_Argument );
    if ( blend && blend->num_axis == num_coords )
    {
       
      FT_Fixed  final_blends[T1_MAX_MM_DESIGNS];


      for ( n = 0; n < blend->num_axis; n++ )
      {
        FT_Long       design  = coords[n];
        FT_Fixed      the_blend;
        PS_DesignMap  map     = blend->design_map + n;
        FT_Long*      designs = map->design_points;
        FT_Fixed*     blends  = map->blend_points;
        FT_Int        before  = -1, after = -1;


        for ( p = 0; p < (FT_UInt)map->num_points; p++ )
        {
          FT_Long  p_design = designs[p];


           
          if ( design == p_design )
          {
            the_blend = blends[p];
            goto Found;
          }

          if ( design < p_design )
          {
            after = p;
            break;
          }

          before = p;
        }

         
        if ( before < 0 )
          the_blend = blends[0];

        else if ( after < 0 )
          the_blend = blends[map->num_points - 1];

        else
          the_blend = FT_MulDiv( design         - designs[before],
                                 blends [after] - blends [before],
                                 designs[after] - designs[before] );

      Found:
        final_blends[n] = the_blend;
      }

      error = T1_Set_MM_Blend( face, num_coords, final_blends );
    }

    return error;
  }
