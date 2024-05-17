  Normalize( FT_F26Dot6      Vx,
             FT_F26Dot6      Vy,
             FT_UnitVector*  R )
  {
    FT_Vector V;


    if ( Vx == 0 && Vy == 0 )
    {
       
       
      return SUCCESS;
    }

    V.x = Vx;
    V.y = Vy;

    FT_Vector_NormLen( &V );

    R->x = (FT_F2Dot14)( V.x / 4 );
    R->y = (FT_F2Dot14)( V.y / 4 );

    return SUCCESS;
  }
