  Current_Ratio( TT_ExecContext  exc )
  {
    if ( !exc->tt_metrics.ratio )
    {
      if ( exc->GS.projVector.y == 0 )
        exc->tt_metrics.ratio = exc->tt_metrics.x_ratio;

      else if ( exc->GS.projVector.x == 0 )
        exc->tt_metrics.ratio = exc->tt_metrics.y_ratio;

      else
      {
        FT_F26Dot6  x, y;


        x = TT_MulFix14( exc->tt_metrics.x_ratio,
                         exc->GS.projVector.x );
        y = TT_MulFix14( exc->tt_metrics.y_ratio,
                         exc->GS.projVector.y );
        exc->tt_metrics.ratio = FT_Hypot( x, y );
      }
    }
    return exc->tt_metrics.ratio;
  }
