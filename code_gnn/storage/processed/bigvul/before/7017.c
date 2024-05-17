  Conic_To( RAS_ARGS Long  cx,
                     Long  cy,
                     Long  x,
                     Long  y )
  {
    Long     y1, y2, y3, x3, ymin, ymax;
    TStates  state_bez;


    ras.arc      = ras.arcs;
    ras.arc[2].x = ras.lastX;
    ras.arc[2].y = ras.lastY;
    ras.arc[1].x = cx;
    ras.arc[1].y = cy;
    ras.arc[0].x = x;
    ras.arc[0].y = y;

    do
    {
      y1 = ras.arc[2].y;
      y2 = ras.arc[1].y;
      y3 = ras.arc[0].y;
      x3 = ras.arc[0].x;

       

      if ( y1 <= y3 )
      {
        ymin = y1;
        ymax = y3;
      }
      else
      {
        ymin = y3;
        ymax = y1;
      }

      if ( y2 < ymin || y2 > ymax )
      {
         
        Split_Conic( ras.arc );
        ras.arc += 2;
      }
      else if ( y1 == y3 )
      {
         
        ras.arc -= 2;
      }
      else
      {
         
         
        state_bez = y1 < y3 ? Ascending_State : Descending_State;
        if ( ras.state != state_bez )
        {
          Bool  o = state_bez == Ascending_State ? IS_BOTTOM_OVERSHOOT( y1 )
                                                 : IS_TOP_OVERSHOOT( y1 );


           
          if ( ras.state != Unknown_State &&
               End_Profile( RAS_VARS o )  )
            goto Fail;

           
          if ( New_Profile( RAS_VARS state_bez, o ) )
            goto Fail;
        }

         
        if ( state_bez == Ascending_State )
        {
          if ( Bezier_Up( RAS_VARS 2, Split_Conic, ras.minY, ras.maxY ) )
            goto Fail;
        }
        else
          if ( Bezier_Down( RAS_VARS 2, Split_Conic, ras.minY, ras.maxY ) )
            goto Fail;
      }

    } while ( ras.arc >= ras.arcs );

    ras.lastX = x3;
    ras.lastY = y3;

    return SUCCESS;

  Fail:
    return FAILURE;
  }
