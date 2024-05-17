  gray_set_cell( RAS_ARG_ TCoord  ex,
                          TCoord  ey )
  {
     
     
     
     
     
     
     
     
     

     
     
    ey -= ras.min_ey;

    if ( ex > ras.max_ex )
      ex = ras.max_ex;

    ex -= ras.min_ex;
    if ( ex < 0 )
      ex = -1;

     
    if ( ex != ras.ex || ey != ras.ey )
    {
       
      if ( !ras.invalid )
        gray_record_cell( RAS_VAR );

      ras.area  = 0;
      ras.cover = 0;
    }

    ras.ex      = ex;
    ras.ey      = ey;
    ras.invalid = ( (unsigned)ey >= (unsigned)ras.count_ey ||
                              ex >= ras.count_ex           );
  }
