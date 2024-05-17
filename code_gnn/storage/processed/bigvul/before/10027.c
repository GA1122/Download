  Clear_Display( void )
  {
    long  bitmap_size = (long)bit.pitch * bit.rows;


    if ( bitmap_size < 0 )
      bitmap_size = -bitmap_size;
    memset( bit.buffer, 0, bitmap_size );
  }
