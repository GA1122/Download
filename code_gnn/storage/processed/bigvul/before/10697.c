  Move_Zp2_Point( TT_ExecContext  exc,
                  FT_UShort       point,
                  FT_F26Dot6      dx,
                  FT_F26Dot6      dy,
                  FT_Bool         touch )
  {
    if ( exc->GS.freeVector.x != 0 )
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
      if ( !( SUBPIXEL_HINTING_MINIMAL    &&
              exc->backward_compatibility ) )
#endif
        exc->zp2.cur[point].x = ADD_LONG( exc->zp2.cur[point].x, dx );

      if ( touch )
        exc->zp2.tags[point] |= FT_CURVE_TAG_TOUCH_X;
    }

    if ( exc->GS.freeVector.y != 0 )
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
      if ( !( SUBPIXEL_HINTING_MINIMAL    &&
              exc->backward_compatibility &&
              exc->iupx_called            &&
              exc->iupy_called            ) )
#endif
        exc->zp2.cur[point].y = ADD_LONG( exc->zp2.cur[point].y, dy );

      if ( touch )
        exc->zp2.tags[point] |= FT_CURVE_TAG_TOUCH_Y;
    }
  }
