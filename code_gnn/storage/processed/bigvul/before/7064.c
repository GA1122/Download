  T1_Set_MM_Blend( T1_Face    face,
                   FT_UInt    num_coords,
                   FT_Fixed*  coords )
  {
    PS_Blend  blend = face->blend;
    FT_Error  error;
    FT_UInt   n, m;


    error = FT_ERR( Invalid_Argument );

    if ( blend && blend->num_axis == num_coords )
    {
       
      for ( n = 0; n < blend->num_designs; n++ )
      {
        FT_Fixed  result = 0x10000L;   


        for ( m = 0; m < blend->num_axis; m++ )
        {
          FT_Fixed  factor;


           
          factor = coords[m];
          if ( factor < 0 )
            factor = 0;
          if ( factor > 0x10000L )
            factor = 0x10000L;

          if ( ( n & ( 1 << m ) ) == 0 )
            factor = 0x10000L - factor;

          result = FT_MulFix( result, factor );
        }
        blend->weight_vector[n] = result;
      }

      error = FT_Err_Ok;
    }

    return error;
  }
