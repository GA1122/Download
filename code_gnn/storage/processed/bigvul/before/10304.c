  gray_move_to( const FT_Vector*  to,
                PWorker           worker )
  {
    TPos  x, y;


     
    gray_record_cell( RAS_VAR );

     
    x = UPSCALE( to->x );
    y = UPSCALE( to->y );

    gray_start_cell( RAS_VAR_ TRUNC( x ), TRUNC( y ) );

    worker->x = x;
    worker->y = y;
    return 0;
  }
