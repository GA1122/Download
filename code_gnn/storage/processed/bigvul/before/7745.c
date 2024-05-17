  TT_Set_MM_Blend( TT_Face    face,
                   FT_UInt    num_coords,
                   FT_Fixed*  coords )
  {
    FT_Error    error = TT_Err_Ok;
    GX_Blend    blend;
    FT_MM_Var*  mmvar;
    FT_UInt     i;
    FT_Memory   memory = face->root.memory;

    enum
    {
      mcvt_retain,
      mcvt_modify,
      mcvt_load

    } manageCvt;


    face->doblend = FALSE;

    if ( face->blend == NULL )
    {
      if ( (error = TT_Get_MM_Var( face, NULL)) != 0 )
        goto Exit;
    }

    blend = face->blend;
    mmvar = blend->mmvar;

    if ( num_coords != mmvar->num_axis )
    {
      error = TT_Err_Invalid_Argument;
      goto Exit;
    }

    for ( i = 0; i < num_coords; ++i )
      if ( coords[i] < -0x00010000L || coords[i] > 0x00010000L )
      {
        error = TT_Err_Invalid_Argument;
        goto Exit;
      }

    if ( blend->glyphoffsets == NULL )
      if ( (error = ft_var_load_gvar( face )) != 0 )
        goto Exit;

    if ( blend->normalizedcoords == NULL )
    {
      if ( FT_NEW_ARRAY( blend->normalizedcoords, num_coords ) )
        goto Exit;

      manageCvt = mcvt_modify;

       
       
       
    }
    else
    {
      manageCvt = mcvt_retain;
      for ( i = 0; i < num_coords; ++i )
      {
        if ( blend->normalizedcoords[i] != coords[i] )
        {
          manageCvt = mcvt_load;
          break;
        }
      }

       
       
       
       
    }

    blend->num_axis = num_coords;
    FT_MEM_COPY( blend->normalizedcoords,
                 coords,
                 num_coords * sizeof ( FT_Fixed ) );

    face->doblend = TRUE;

    if ( face->cvt != NULL )
    {
      switch ( manageCvt )
      {
      case mcvt_load:
         
         
        FT_FREE( face->cvt );
        face->cvt = NULL;

        tt_face_load_cvt( face, face->root.stream );
        break;

      case mcvt_modify:
         
         
        tt_face_vary_cvt( face, face->root.stream );
        break;

      case mcvt_retain:
         
        break;
      }
    }

  Exit:
    return error;
  }
