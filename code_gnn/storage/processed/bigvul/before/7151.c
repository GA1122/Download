  cf2_hint_isTop( const CF2_Hint  hint )
  {
    return (FT_Bool)( ( hint->flags                    &
                        ( CF2_PairTop | CF2_GhostTop ) ) != 0 );
  }
