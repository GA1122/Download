  cf2_hint_isLocked( const CF2_Hint  hint )
  {
    return (FT_Bool)( ( hint->flags & CF2_Locked ) != 0 );
  }
