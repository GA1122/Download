  psh_compute_dir( FT_Pos  dx,
                   FT_Pos  dy )
  {
    FT_Pos  ax, ay;
    int     result = PSH_DIR_NONE;


    ax = ( dx >= 0 ) ? dx : -dx;
    ay = ( dy >= 0 ) ? dy : -dy;

    if ( ay * 12 < ax )
    {
       
      result = ( dx >= 0 ) ? PSH_DIR_RIGHT : PSH_DIR_LEFT;
    }
    else if ( ax * 12 < ay )
    {
       
      result = ( dy >= 0 ) ? PSH_DIR_UP : PSH_DIR_DOWN;
    }

    return result;
  }