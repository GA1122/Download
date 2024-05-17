  t1_allocate_blend( T1_Face  face,
                     FT_UInt  num_designs,
                     FT_UInt  num_axis )
  {
    PS_Blend   blend;
    FT_Memory  memory = face->root.memory;
    FT_Error   error  = FT_Err_Ok;


    blend = face->blend;
    if ( !blend )
    {
      if ( FT_NEW( blend ) )
        goto Exit;

      blend->num_default_design_vector = 0;

      face->blend = blend;
    }

     
    if ( num_designs > 0 )
    {
      if ( blend->num_designs == 0 )
      {
        FT_UInt  nn;


         
        if ( FT_NEW_ARRAY( blend->font_infos[1], num_designs     ) ||
             FT_NEW_ARRAY( blend->privates  [1], num_designs     ) ||
             FT_NEW_ARRAY( blend->bboxes    [1], num_designs     ) ||
             FT_NEW_ARRAY( blend->weight_vector, num_designs * 2 ) )
          goto Exit;

        blend->default_weight_vector = blend->weight_vector + num_designs;

        blend->font_infos[0] = &face->type1.font_info;
        blend->privates  [0] = &face->type1.private_dict;
        blend->bboxes    [0] = &face->type1.font_bbox;

        for ( nn = 2; nn <= num_designs; nn++ )
        {
          blend->font_infos[nn] = blend->font_infos[nn - 1] + 1;
          blend->privates  [nn] = blend->privates  [nn - 1] + 1;
          blend->bboxes    [nn] = blend->bboxes    [nn - 1] + 1;
        }

        blend->num_designs = num_designs;
      }
      else if ( blend->num_designs != num_designs )
        goto Fail;
    }

     
    if ( num_axis > 0 )
    {
      if ( blend->num_axis != 0 && blend->num_axis != num_axis )
        goto Fail;

      blend->num_axis = num_axis;
    }

     
    num_designs = blend->num_designs;
    num_axis    = blend->num_axis;
    if ( num_designs && num_axis && blend->design_pos[0] == 0 )
    {
      FT_UInt  n;


      if ( FT_NEW_ARRAY( blend->design_pos[0], num_designs * num_axis ) )
        goto Exit;

      for ( n = 1; n < num_designs; n++ )
        blend->design_pos[n] = blend->design_pos[0] + num_axis * n;
    }

  Exit:
    return error;

  Fail:
    error = FT_THROW( Invalid_File_Format );
    goto Exit;
  }
