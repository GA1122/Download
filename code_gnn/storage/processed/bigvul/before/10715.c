  TT_Done_Context( TT_ExecContext  exec )
  {
    FT_Memory  memory = exec->memory;


     
    exec->maxPoints   = 0;
    exec->maxContours = 0;

     
    FT_FREE( exec->stack );
    exec->stackSize = 0;

     
    FT_FREE( exec->callStack );
    exec->callSize = 0;
    exec->callTop  = 0;

     
    FT_FREE( exec->glyphIns );
    exec->glyphSize = 0;

    exec->size = NULL;
    exec->face = NULL;

    FT_FREE( exec );
  }