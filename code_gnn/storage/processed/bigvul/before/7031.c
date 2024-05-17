  Insert_Y_Turn( RAS_ARGS Int  y )
  {
    PLong  y_turns;
    Int    n;


    n       = ras.numTurns - 1;
    y_turns = ras.sizeBuff - ras.numTurns;

     
    while ( n >= 0 && y < y_turns[n] )
      n--;

     
    if ( n >= 0 && y > y_turns[n] )
      while ( n >= 0 )
      {
        Int  y2 = (Int)y_turns[n];


        y_turns[n] = y;
        y = y2;
        n--;
      }

    if ( n < 0 )
    {
      ras.maxBuff--;
      if ( ras.maxBuff <= ras.top )
      {
        ras.error = FT_THROW( Overflow );
        return FAILURE;
      }
      ras.numTurns++;
      ras.sizeBuff[-ras.numTurns] = y;
    }

    return SUCCESS;
  }
