  TT_VecLen( FT_F26Dot6  X,
             FT_F26Dot6  Y )
  {
    FT_Vector  v;


    v.x = X;
    v.y = Y;

    return FT_Vector_Length( &v );
  }
