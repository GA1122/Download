  cf2_doFlex( CF2_Stack       opStack,
              CF2_Fixed*      curX,
              CF2_Fixed*      curY,
              CF2_GlyphPath   glyphPath,
              const FT_Bool*  readFromStack,
              FT_Bool         doConditionalLastRead )
  {
    CF2_Fixed  vals[14];
    CF2_UInt   index;
    FT_Bool    isHFlex;
    CF2_Int    top, i, j;


    vals[0] = *curX;
    vals[1] = *curY;
    index   = 0;
    isHFlex = readFromStack[9] == FALSE;
    top     = isHFlex ? 9 : 10;

    for ( i = 0; i < top; i++ )
    {
      vals[i + 2] = vals[i];
      if ( readFromStack[i] )
        vals[i + 2] += cf2_stack_getReal( opStack, index++ );
    }

    if ( isHFlex )
      vals[9 + 2] = *curY;

    if ( doConditionalLastRead )
    {
      FT_Bool    lastIsX = (FT_Bool)( cf2_fixedAbs( vals[10] - *curX ) >
                                        cf2_fixedAbs( vals[11] - *curY ) );
      CF2_Fixed  lastVal = cf2_stack_getReal( opStack, index );


      if ( lastIsX )
      {
        vals[12] = vals[10] + lastVal;
        vals[13] = *curY;
      }
      else
      {
        vals[12] = *curX;
        vals[13] = vals[11] + lastVal;
      }
    }
    else
    {
      if ( readFromStack[10] )
        vals[12] = vals[10] + cf2_stack_getReal( opStack, index++ );
      else
        vals[12] = *curX;

      if ( readFromStack[11] )
        vals[13] = vals[11] + cf2_stack_getReal( opStack, index );
      else
        vals[13] = *curY;
    }

    for ( j = 0; j < 2; j++ )
      cf2_glyphpath_curveTo( glyphPath, vals[j * 6 + 2],
                                        vals[j * 6 + 3],
                                        vals[j * 6 + 4],
                                        vals[j * 6 + 5],
                                        vals[j * 6 + 6],
                                        vals[j * 6 + 7] );

    cf2_stack_clear( opStack );

    *curX = vals[12];
    *curY = vals[13];
  }
