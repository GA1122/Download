  T1_Done_Blend( T1_Face  face )
  {
    FT_Memory  memory = face->root.memory;
    PS_Blend   blend  = face->blend;


    if ( blend )
    {
      FT_UInt  num_designs = blend->num_designs;
      FT_UInt  num_axis    = blend->num_axis;
      FT_UInt  n;


       
      FT_FREE( blend->design_pos[0] );
      for ( n = 1; n < num_designs; n++ )
        blend->design_pos[n] = NULL;

       
      FT_FREE( blend->privates[1] );
      FT_FREE( blend->font_infos[1] );
      FT_FREE( blend->bboxes[1] );

      for ( n = 0; n < num_designs; n++ )
      {
        blend->privates  [n] = NULL;
        blend->font_infos[n] = NULL;
        blend->bboxes    [n] = NULL;
      }

       
      FT_FREE( blend->weight_vector );
      blend->default_weight_vector = NULL;

       
      for ( n = 0; n < num_axis; n++ )
        FT_FREE( blend->axis_names[n] );

       
      for ( n = 0; n < num_axis; n++ )
      {
        PS_DesignMap  dmap = blend->design_map + n;


        FT_FREE( dmap->design_points );
        dmap->num_points = 0;
      }

      FT_FREE( face->blend );
    }
  }
