  tt_size_init_bytecode( FT_Size  ftsize,
                         FT_Bool  pedantic )
  {
    FT_Error   error;
    TT_Size    size = (TT_Size)ftsize;
    TT_Face    face = (TT_Face)ftsize->face;
    FT_Memory  memory = face->root.memory;

    FT_UShort       n_twilight;
    TT_MaxProfile*  maxp = &face->max_profile;


     
    FT_FREE( size->function_defs );
    FT_FREE( size->instruction_defs );
    FT_FREE( size->cvt );
    FT_FREE( size->storage );

    if ( size->context )
      TT_Done_Context( size->context );
    tt_glyphzone_done( &size->twilight );

    size->bytecode_ready = -1;
    size->cvt_ready      = -1;

    size->context = TT_New_Context( (TT_Driver)face->root.driver );

    size->max_function_defs    = maxp->maxFunctionDefs;
    size->max_instruction_defs = maxp->maxInstructionDefs;

    size->num_function_defs    = 0;
    size->num_instruction_defs = 0;

    size->max_func = 0;
    size->max_ins  = 0;

    size->cvt_size     = face->cvt_size;
    size->storage_size = maxp->maxStorage;

     
    {
      TT_Size_Metrics*  metrics = &size->ttmetrics;


      metrics->rotated   = FALSE;
      metrics->stretched = FALSE;

       
      metrics->compensations[0] = 0;    
      metrics->compensations[1] = 0;    
      metrics->compensations[2] = 0;    
      metrics->compensations[3] = 0;    
    }

     
    if ( FT_NEW_ARRAY( size->function_defs,    size->max_function_defs    ) ||
         FT_NEW_ARRAY( size->instruction_defs, size->max_instruction_defs ) ||
         FT_NEW_ARRAY( size->cvt,              size->cvt_size             ) ||
         FT_NEW_ARRAY( size->storage,          size->storage_size         ) )
      goto Exit;

     
    n_twilight = maxp->maxTwilightPoints;

     
    n_twilight += 4;

    error = tt_glyphzone_new( memory, n_twilight, 0, &size->twilight );
    if ( error )
      goto Exit;

    size->twilight.n_points = n_twilight;

    size->GS = tt_default_graphics_state;

     
    {
      FT_Library  library = face->root.driver->root.library;


      face->interpreter = (TT_Interpreter)
                            library->debug_hooks[FT_DEBUG_HOOK_TRUETYPE];
      if ( !face->interpreter )
        face->interpreter = (TT_Interpreter)TT_RunIns;
    }

     

     
     
     
     
     
     
    error = tt_size_run_fpgm( size, pedantic );
    return error;

  Exit:
    if ( error )
      tt_size_done_bytecode( ftsize );

    return error;
  }
