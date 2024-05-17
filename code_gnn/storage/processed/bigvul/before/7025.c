  Horizontal_Gray_Sweep_Drop( RAS_ARGS Short       y,
                                       FT_F26Dot6  x1,
                                       FT_F26Dot6  x2,
                                       PProfile    left,
                                       PProfile    right )
  {
    Long   e1, e2;
    PByte  pixel;


     

    e1 = CEILING( x1 );
    e2 = FLOOR  ( x2 );

    if ( e1 > e2 )
    {
      Int  dropOutControl = left->flags & 7;


      if ( e1 == e2 + ras.precision )
      {
        switch ( dropOutControl )
        {
        case 0:  
          e1 = e2;
          break;

        case 4:  
          e1 = FLOOR( ( x1 + x2 - 1 ) / 2 + ras.precision_half );
          break;

        case 1:  
        case 5:  
           

           
          if ( left->next == right && left->height <= 0 )
            return;

           
          if ( right->next == left && left->start == y )
            return;

          if ( dropOutControl == 1 )
            e1 = e2;
          else
            e1 = FLOOR( ( x1 + x2 - 1 ) / 2 + ras.precision_half );

          break;

        default:  
          return;   
        }
      }
      else
        return;
    }

    if ( e1 >= 0 )
    {
      Byte  color;


      if ( x2 - x1 >= ras.precision_half )
        color = ras.grays[2];
      else
        color = ras.grays[1];

      e1 = TRUNC( e1 ) / 2;
      if ( e1 < ras.target.rows )
      {
        pixel = ras.gTarget - e1 * ras.target.pitch + y / 2;
        if ( ras.target.pitch > 0 )
          pixel += ( ras.target.rows - 1 ) * ras.target.pitch;

        if ( pixel[0] == ras.grays[0] )
          pixel[0] = color;
      }
    }
  }
