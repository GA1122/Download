  Ins_ISECT( TT_ExecContext  exc,
             FT_Long*        args )
  {
    FT_UShort   point,
                a0, a1,
                b0, b1;

    FT_F26Dot6  discriminant, dotproduct;

    FT_F26Dot6  dx,  dy,
                dax, day,
                dbx, dby;

    FT_F26Dot6  val;

    FT_Vector   R;


    point = (FT_UShort)args[0];

    a0 = (FT_UShort)args[1];
    a1 = (FT_UShort)args[2];
    b0 = (FT_UShort)args[3];
    b1 = (FT_UShort)args[4];

    if ( BOUNDS( b0,    exc->zp0.n_points ) ||
         BOUNDS( b1,    exc->zp0.n_points ) ||
         BOUNDS( a0,    exc->zp1.n_points ) ||
         BOUNDS( a1,    exc->zp1.n_points ) ||
         BOUNDS( point, exc->zp2.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

     

    dbx = SUB_LONG( exc->zp0.cur[b1].x, exc->zp0.cur[b0].x );
    dby = SUB_LONG( exc->zp0.cur[b1].y, exc->zp0.cur[b0].y );

    dax = SUB_LONG( exc->zp1.cur[a1].x, exc->zp1.cur[a0].x );
    day = SUB_LONG( exc->zp1.cur[a1].y, exc->zp1.cur[a0].y );

    dx = SUB_LONG( exc->zp0.cur[b0].x, exc->zp1.cur[a0].x );
    dy = SUB_LONG( exc->zp0.cur[b0].y, exc->zp1.cur[a0].y );

    discriminant = ADD_LONG( FT_MulDiv( dax, NEG_LONG( dby ), 0x40 ),
                             FT_MulDiv( day, dbx, 0x40 ) );
    dotproduct   = ADD_LONG( FT_MulDiv( dax, dbx, 0x40 ),
                             FT_MulDiv( day, dby, 0x40 ) );

     
     
     
     
     
     
     
     
    if ( MUL_LONG( 19, FT_ABS( discriminant ) ) > FT_ABS( dotproduct ) )
    {
      val = ADD_LONG( FT_MulDiv( dx, NEG_LONG( dby ), 0x40 ),
                      FT_MulDiv( dy, dbx, 0x40 ) );

      R.x = FT_MulDiv( val, dax, discriminant );
      R.y = FT_MulDiv( val, day, discriminant );

       
      exc->zp2.cur[point].x = ADD_LONG( exc->zp1.cur[a0].x, R.x );
      exc->zp2.cur[point].y = ADD_LONG( exc->zp1.cur[a0].y, R.y );
    }
    else
    {
       

       
      exc->zp2.cur[point].x =
        ADD_LONG( ADD_LONG( exc->zp1.cur[a0].x, exc->zp1.cur[a1].x ),
                  ADD_LONG( exc->zp0.cur[b0].x, exc->zp0.cur[b1].x ) ) / 4;
      exc->zp2.cur[point].y =
        ADD_LONG( ADD_LONG( exc->zp1.cur[a0].y, exc->zp1.cur[a1].y ),
                  ADD_LONG( exc->zp0.cur[b0].y, exc->zp0.cur[b1].y ) ) / 4;
    }

    exc->zp2.tags[point] |= FT_CURVE_TAG_TOUCH_BOTH;
  }
