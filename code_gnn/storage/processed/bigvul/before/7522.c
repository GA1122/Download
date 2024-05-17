  tt_size_run_fpgm( TT_Size  size,
                    FT_Bool  pedantic )
  {
    TT_Face         face = (TT_Face)size->root.face;
    TT_ExecContext  exec;
    FT_Error        error;


    exec = size->context;

    error = TT_Load_Context( exec, face, size );
    if ( error )
      return error;

    exec->callTop = 0;
    exec->top     = 0;

    exec->period    = 64;
    exec->phase     = 0;
    exec->threshold = 0;

    exec->instruction_trap = FALSE;
    exec->F_dot_P          = 0x4000L;

    exec->pedantic_hinting = pedantic;

    {
      FT_Size_Metrics*  metrics    = &exec->metrics;
      TT_Size_Metrics*  tt_metrics = &exec->tt_metrics;


      metrics->x_ppem   = 0;
      metrics->y_ppem   = 0;
      metrics->x_scale  = 0;
      metrics->y_scale  = 0;

      tt_metrics->ppem  = 0;
      tt_metrics->scale = 0;
      tt_metrics->ratio = 0x10000L;
    }

     
    TT_Set_CodeRange( exec,
                      tt_coderange_font,
                      face->font_program,
                      (FT_Long)face->font_program_size );

     
    TT_Clear_CodeRange( exec, tt_coderange_cvt );
    TT_Clear_CodeRange( exec, tt_coderange_glyph );

    if ( face->font_program_size > 0 )
    {
      TT_Goto_CodeRange( exec, tt_coderange_font, 0 );

      FT_TRACE4(( "Executing `fpgm' table.\n" ));
      error = face->interpreter( exec );
#ifdef FT_DEBUG_LEVEL_TRACE
      if ( error )
        FT_TRACE4(( "  interpretation failed with error code 0x%x\n",
                    error ));
#endif
    }
    else
      error = FT_Err_Ok;

    size->bytecode_ready = error;

    if ( !error )
      TT_Save_Context( exec, size );

    return error;
  }
