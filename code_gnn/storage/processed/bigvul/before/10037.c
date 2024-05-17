  Reset_Scale( int  pointSize )
  {
    (void)FT_Set_Char_Size( face, pointSize << 6, pointSize << 6, res, res );
  }
