  cf2_hint_isSynthetic( const CF2_Hint  hint )
  {
    return (FT_Bool)( ( hint->flags & CF2_Synthetic ) != 0 );
  }
