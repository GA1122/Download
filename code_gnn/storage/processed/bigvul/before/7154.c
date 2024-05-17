  cf2_hintmap_adjustHints( CF2_HintMap  hintmap )
  {
    size_t  i, j;


    cf2_arrstack_clear( hintmap->hintMoves );       

     

    for ( i = 0; i < hintmap->count; i++ )
    {
      FT_Bool  isPair = cf2_hint_isPair( &hintmap->edge[i] );


       
      j = isPair ? i + 1 : i;

      FT_ASSERT( j < hintmap->count );
      FT_ASSERT( cf2_hint_isValid( &hintmap->edge[i] ) );
      FT_ASSERT( cf2_hint_isValid( &hintmap->edge[j] ) );
      FT_ASSERT( cf2_hint_isLocked( &hintmap->edge[i] ) ==
                   cf2_hint_isLocked( &hintmap->edge[j] ) );

      if ( !cf2_hint_isLocked( &hintmap->edge[i] ) )
      {
         
        CF2_Fixed  fracDown = cf2_fixedFraction( hintmap->edge[i].dsCoord );
        CF2_Fixed  fracUp   = cf2_fixedFraction( hintmap->edge[j].dsCoord );

         
        CF2_Fixed  downMoveDown = 0 - fracDown;
        CF2_Fixed  upMoveDown   = 0 - fracUp;
        CF2_Fixed  downMoveUp   = fracDown == 0
                                    ? 0
                                    : cf2_intToFixed( 1 ) - fracDown;
        CF2_Fixed  upMoveUp     = fracUp == 0
                                    ? 0
                                    : cf2_intToFixed( 1 ) - fracUp;

         
        CF2_Fixed  moveUp   = FT_MIN( downMoveUp, upMoveUp );
         
        CF2_Fixed  moveDown = FT_MAX( downMoveDown, upMoveDown );

         
        CF2_Fixed  move;

        CF2_Fixed  downMinCounter = CF2_MIN_COUNTER;
        CF2_Fixed  upMinCounter   = CF2_MIN_COUNTER;
        FT_Bool    saveEdge       = FALSE;


         
         
         
#if 0
        if ( i == 0                                        ||
             cf2_hint_isSynthetic( &hintmap->edge[i - 1] ) )
          downMinCounter = 0;

        if ( j >= hintmap->count - 1                       ||
             cf2_hint_isSynthetic( &hintmap->edge[j + 1] ) )
          upMinCounter = 0;
#endif

         
         
         
        if ( j >= hintmap->count - 1                            ||
             hintmap->edge[j + 1].dsCoord >=
               hintmap->edge[j].dsCoord + moveUp + upMinCounter )
        {
           
          if ( i == 0                                                 ||
               hintmap->edge[i - 1].dsCoord <=
                 hintmap->edge[i].dsCoord + moveDown - downMinCounter )
          {
             
            move = ( -moveDown < moveUp ) ? moveDown : moveUp;   
          }
          else
            move = moveUp;
        }
        else
        {
           
          if ( i == 0                                                 ||
               hintmap->edge[i - 1].dsCoord <=
                 hintmap->edge[i].dsCoord + moveDown - downMinCounter )
          {
            move     = moveDown;
             
            saveEdge = (FT_Bool)( moveUp < -moveDown );
          }
          else
          {
             
             
            move     = 0;
            saveEdge = TRUE;
          }
        }

         
         
         
         
        if ( saveEdge                                    &&
             j < hintmap->count - 1                      &&
             !cf2_hint_isLocked( &hintmap->edge[j + 1] ) )
        {
          CF2_HintMoveRec  savedMove;


          savedMove.j      = j;
           
          savedMove.moveUp = moveUp - move;

          cf2_arrstack_push( hintmap->hintMoves, &savedMove );
        }

         
        hintmap->edge[i].dsCoord += move;
        if ( isPair )
          hintmap->edge[j].dsCoord += move;
      }

       
      FT_ASSERT( i == 0                                                   ||
                 hintmap->edge[i - 1].dsCoord <= hintmap->edge[i].dsCoord );
      FT_ASSERT( i < j                                                ||
                 hintmap->edge[i].dsCoord <= hintmap->edge[j].dsCoord );

       
      if ( i > 0 )
      {
        if ( hintmap->edge[i].csCoord != hintmap->edge[i - 1].csCoord )
          hintmap->edge[i - 1].scale =
            FT_DivFix(
              hintmap->edge[i].dsCoord - hintmap->edge[i - 1].dsCoord,
              hintmap->edge[i].csCoord - hintmap->edge[i - 1].csCoord );
      }

      if ( isPair )
      {
        if ( hintmap->edge[j].csCoord != hintmap->edge[j - 1].csCoord )
          hintmap->edge[j - 1].scale =
            FT_DivFix(
              hintmap->edge[j].dsCoord - hintmap->edge[j - 1].dsCoord,
              hintmap->edge[j].csCoord - hintmap->edge[j - 1].csCoord );

        i += 1;      
      }
    }

     
     
    for ( i = cf2_arrstack_size( hintmap->hintMoves ); i > 0; i-- )
    {
      CF2_HintMove  hintMove = (CF2_HintMove)
                      cf2_arrstack_getPointer( hintmap->hintMoves, i - 1 );


      j = hintMove->j;

       
      FT_ASSERT( j < hintmap->count - 1 );

       
      if ( hintmap->edge[j + 1].dsCoord >=
             hintmap->edge[j].dsCoord + hintMove->moveUp + CF2_MIN_COUNTER )
      {
         
        hintmap->edge[j].dsCoord += hintMove->moveUp;

        if ( cf2_hint_isPair( &hintmap->edge[j] ) )
        {
          FT_ASSERT( j > 0 );
          hintmap->edge[j - 1].dsCoord += hintMove->moveUp;
        }
      }
    }
  }
