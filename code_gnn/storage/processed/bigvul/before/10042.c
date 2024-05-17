  event_angle_change( int  delta )
  {
    double    radian;
    FT_Fixed  cosinus;
    FT_Fixed  sinus;


    status.angle = ( status.angle + delta ) % 360;

    if ( status.angle == 0 )
    {
      status.sc.matrix = NULL;

      return;
    }

    status.sc.matrix = &status.trans_matrix;

    if ( status.angle < 0 )
      status.angle += 360;

    radian  = status.angle * 3.14159 / 180.0;
    cosinus = (FT_Fixed)( cos( radian ) * 65536.0 );
    sinus   = (FT_Fixed)( sin( radian ) * 65536.0 );

    status.trans_matrix.xx = cosinus;
    status.trans_matrix.yx = sinus;
    status.trans_matrix.xy = -sinus;
    status.trans_matrix.yy = cosinus;
  }
