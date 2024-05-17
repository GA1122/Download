  cf2_hint_isPair( const CF2_Hint  hint )
  {
    return (FT_Bool)( ( hint->flags                      &
                        ( CF2_PairBottom | CF2_PairTop ) ) != 0 );
  }
