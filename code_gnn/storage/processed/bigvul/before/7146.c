  cf2_hint_isBottom( const CF2_Hint  hint )
  {
    return (FT_Bool)( ( hint->flags                          &
                        ( CF2_PairBottom | CF2_GhostBottom ) ) != 0 );
  }
