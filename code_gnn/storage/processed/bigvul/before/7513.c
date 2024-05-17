  tt_face_init( FT_Stream      stream,
                FT_Face        ttface,       
                FT_Int         face_index,
                FT_Int         num_params,
                FT_Parameter*  params )
  {
    FT_Error      error;
    FT_Library    library;
    SFNT_Service  sfnt;
    TT_Face       face = (TT_Face)ttface;


    FT_TRACE2(( "TTF driver\n" ));

    library = ttface->driver->root.library;

    sfnt = (SFNT_Service)FT_Get_Module_Interface( library, "sfnt" );
    if ( !sfnt )
    {
      FT_ERROR(( "tt_face_init: cannot access `sfnt' module\n" ));
      error = FT_THROW( Missing_Module );
      goto Exit;
    }

     
    if ( FT_STREAM_SEEK( 0 ) )
      goto Exit;

     
    FT_TRACE2(( "  " ));
    error = sfnt->init_face( stream, face, face_index, num_params, params );

     
    stream = face->root.stream;

    if ( error )
      goto Exit;

     
     
     
    if ( face->format_tag != 0x00010000L &&     
         face->format_tag != 0x00020000L &&     
         face->format_tag != TTAG_true   )      
    {
      FT_TRACE2(( "  not a TTF font\n" ));
      goto Bad_Format;
    }

#ifdef TT_USE_BYTECODE_INTERPRETER
    ttface->face_flags |= FT_FACE_FLAG_HINTER;
#endif

     
    if ( face_index < 0 )
      return FT_Err_Ok;

     
    error = sfnt->load_face( stream, face, face_index, num_params, params );
    if ( error )
      goto Exit;

    if ( tt_check_trickyness( ttface ) )
      ttface->face_flags |= FT_FACE_FLAG_TRICKY;

    error = tt_face_load_hdmx( face, stream );
    if ( error )
      goto Exit;

    if ( FT_IS_SCALABLE( ttface ) )
    {
#ifdef FT_CONFIG_OPTION_INCREMENTAL
      if ( !ttface->internal->incremental_interface )
#endif
      {
        error = tt_face_load_loca( face, stream );

         
         
        if ( face->glyf_len && FT_ERR_EQ( error, Table_Missing ) )
          goto Exit;
        if ( error )
          goto Exit;
      }

       
      error = tt_face_load_cvt( face, stream );
      if ( error && FT_ERR_NEQ( error, Table_Missing ) )
        goto Exit;

      error = tt_face_load_fpgm( face, stream );
      if ( error && FT_ERR_NEQ( error, Table_Missing ) )
        goto Exit;

      error = tt_face_load_prep( face, stream );
      if ( error && FT_ERR_NEQ( error, Table_Missing ) )
        goto Exit;

       
#ifdef FT_CONFIG_OPTION_INCREMENTAL
      if ( !ttface->internal->incremental_interface )
#endif
      {
        if ( ttface->num_fixed_sizes          &&
             face->glyph_locations            &&
             tt_check_single_notdef( ttface ) )
        {
          FT_TRACE5(( "tt_face_init:"
                      " Only the `.notdef' glyph has an outline.\n"
                      "             "
                      " Resetting scalable flag to FALSE.\n" ));

          ttface->face_flags &= ~FT_FACE_FLAG_SCALABLE;
        }
      }
    }

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT

    {
      FT_Int  instance_index = face_index >> 16;


      if ( FT_HAS_MULTIPLE_MASTERS( ttface ) &&
           instance_index > 0                )
      {
        error = TT_Get_MM_Var( face, NULL );
        if ( error )
          goto Exit;

        if ( face->blend->mmvar->namedstyle )
        {
          FT_Memory  memory = ttface->memory;

          FT_Var_Named_Style*  named_style;
          FT_String*           style_name;


           
          named_style = face->blend->mmvar->namedstyle + instance_index - 1;
          error = sfnt->get_name( face,
                                  (FT_UShort)named_style->strid,
                                  &style_name );
          if ( error )
            goto Exit;

           
          if ( face->root.style_name )
            FT_FREE( face->root.style_name );
          face->root.style_name = style_name;

           
          error = TT_Set_Var_Design( face,
                                     face->blend->mmvar->num_axis,
                                     named_style->coords );
          if ( error )
            goto Exit;

          tt_apply_mvar( face );
        }
      }
    }

#endif  

     
    TT_Init_Glyph_Loading( face );

  Exit:
    return error;

  Bad_Format:
    error = FT_THROW( Unknown_File_Format );
    goto Exit;
  }
