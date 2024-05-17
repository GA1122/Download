  Process_Event( grEvent*  event )
  {
    int  i, axis;


    switch ( event->key )
    {
    case grKeyEsc:             
    case grKEY( 'q' ):
      return 0;

    case grKeyF1:
    case grKEY( '?' ):
      Help();
      return 1;

     

    case grKEY( 'a' ):
      antialias = !antialias;
      new_header = antialias ? (char *)"anti-aliasing is now on"
                             : (char *)"anti-aliasing is now off";
      return 1;

    case grKEY( 'b' ):
      use_sbits  = !use_sbits;
      new_header = use_sbits
                     ? (char *)"embedded bitmaps are now used if available"
                     : (char *)"embedded bitmaps are now ignored";
      return 1;

    case grKEY( 'n' ):
    case grKEY( 'p' ):
      return (int)event->key;

    case grKEY( 'l' ):
      low_prec = !low_prec;
      new_header = low_prec
                     ? (char *)"rendering precision is now forced to low"
                     : (char *)"rendering precision is now normal";
      break;

    case grKEY( 'h' ):
      hinted = !hinted;
      new_header = hinted ? (char *)"glyph hinting is now active"
                          : (char *)"glyph hinting is now ignored";
      break;

    case grKEY( ' ' ):
      render_mode ^= 1;
      new_header = render_mode ? (char *)"rendering all glyphs in font"
                               : (char *)"rendering test text string";
      break;

     

    case grKeyF3:
      i = -20;
      axis = 0;
      goto Do_Axis;

    case grKeyF4:
      i = 20;
      axis = 0;
      goto Do_Axis;

    case grKeyF5:
      i = -20;
      axis = 1;
      goto Do_Axis;

    case grKeyF6:
      i = 20;
      axis = 1;
      goto Do_Axis;

    case grKeyF7:
      i = -20;
      axis = 2;
      goto Do_Axis;

    case grKeyF8:
      i = 20;
      axis = 2;
      goto Do_Axis;

     

    case grKeyPageUp:
      i = 10;
      goto Do_Scale;

    case grKeyPageDown:
      i = -10;
      goto Do_Scale;

    case grKeyUp:
      i = 1;
      goto Do_Scale;

    case grKeyDown:
      i = -1;
      goto Do_Scale;

     

    case grKeyLeft:
      i = -1;
      goto Do_Glyph;

    case grKeyRight:
      i = 1;
      goto Do_Glyph;

    case grKeyF9:
      i = -100;
      goto Do_Glyph;

    case grKeyF10:
      i = 100;
      goto Do_Glyph;

    case grKeyF11:
      i = -1000;
      goto Do_Glyph;

    case grKeyF12:
      i = 1000;
      goto Do_Glyph;

    default:
      ;
    }
    return 1;

  Do_Axis:
    if ( axis < (int)multimaster->num_axis )
    {
      FT_Var_Axis* a   = multimaster->axis + axis;
      FT_Fixed     pos = design_pos[axis];

       
       
       
       
      pos += FT_MulDiv( i, a->maximum-a->minimum, 1000 );
      if ( pos < a->minimum ) pos = a->minimum;
      if ( pos > a->maximum ) pos = a->maximum;

      design_pos[axis] = pos;

      FT_Set_Var_Design_Coordinates( face, multimaster->num_axis, design_pos );
    }
    return 1;

  Do_Scale:
    ptsize += i;
    if ( ptsize < 1 )         ptsize = 1;
    if ( ptsize > MAXPTSIZE ) ptsize = MAXPTSIZE;
    return 1;

  Do_Glyph:
    Num += i;
    if ( Num < 0 )           Num = 0;
    if ( Num >= num_glyphs ) Num = num_glyphs - 1;
    return 1;
  }
