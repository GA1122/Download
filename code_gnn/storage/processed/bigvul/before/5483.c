  static Bool  Normalize( EXEC_OPS TT_F26Dot6      Vx,
                                   TT_F26Dot6      Vy,
                                   TT_UnitVector*  R )
  {
    TT_F26Dot6  W;
    Bool        S1, S2;

    if ( ABS( Vx ) < 0x10000L && ABS( Vy ) < 0x10000L )
    {
      Vx *= 0x100;
      Vy *= 0x100;

      W = Norm( Vx, Vy );

      if ( W == 0 )
      {
         
         
        return SUCCESS;
      }

      R->x = (TT_F2Dot14)MulDiv_Round( Vx, 0x4000L, W );
      R->y = (TT_F2Dot14)MulDiv_Round( Vy, 0x4000L, W );

      return SUCCESS;
    }

    W = Norm( Vx, Vy );

    if ( W <= 0 )
    {
      CUR.error = TT_Err_Divide_By_Zero;
      return FAILURE;
    }

    Vx = MulDiv_Round( Vx, 0x4000L, W );
    Vy = MulDiv_Round( Vy, 0x4000L, W );

    W = Vx * Vx + Vy * Vy;

     
     

    if ( Vx < 0 )
    {
      Vx = -Vx;
      S1 = TRUE;
    }
    else
      S1 = FALSE;

    if ( Vy < 0 )
    {
      Vy = -Vy;
      S2 = TRUE;
    }
    else
      S2 = FALSE;

    while ( W < 0x10000000L )
    {
       
      if ( Vx < Vy )
        Vx++;
      else
        Vy++;

      W = Vx * Vx + Vy * Vy;
    }

    while ( W >= 0x10004000L )
    {
       
      if ( Vx < Vy )
        Vx--;
      else
        Vy--;

      W = Vx * Vx + Vy * Vy;
    }

     
     

    if ( S1 )
      Vx = -Vx;

    if ( S2 )
      Vy = -Vy;

    R->x = (TT_F2Dot14)Vx;    
    R->y = (TT_F2Dot14)Vy;    

    return SUCCESS;
  }
