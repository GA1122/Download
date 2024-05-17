  cf2_hintmap_insertHint( CF2_HintMap  hintmap,
                          CF2_Hint     bottomHintEdge,
                          CF2_Hint     topHintEdge )
  {
    CF2_UInt  indexInsert;

     
    FT_Bool   isPair         = TRUE;
    CF2_Hint  firstHintEdge  = bottomHintEdge;
    CF2_Hint  secondHintEdge = topHintEdge;


     
     
    FT_ASSERT( cf2_hint_isValid( bottomHintEdge ) ||
               cf2_hint_isValid( topHintEdge )    );

     
    if ( !cf2_hint_isValid( bottomHintEdge ) )
    {
       
      firstHintEdge = topHintEdge;
      isPair        = FALSE;
    }
    else if ( !cf2_hint_isValid( topHintEdge ) )
    {
       
      isPair = FALSE;
    }

     
    FT_ASSERT( !isPair                                         ||
               topHintEdge->csCoord >= bottomHintEdge->csCoord );

     
    indexInsert = 0;
    for ( ; indexInsert < hintmap->count; indexInsert++ )
    {
      if ( hintmap->edge[indexInsert].csCoord >= firstHintEdge->csCoord )
        break;
    }

     
    if ( indexInsert < hintmap->count )
    {
       
       
      if ( hintmap->edge[indexInsert].csCoord == firstHintEdge->csCoord )
        return;  

       
      if ( isPair                                                        &&
           hintmap->edge[indexInsert].csCoord <= secondHintEdge->csCoord )
        return;  

       
      if ( cf2_hint_isPairTop( &hintmap->edge[indexInsert] ) )
        return;  
    }

     
    if ( cf2_hintmap_isValid( hintmap->initialHintMap ) &&
         !cf2_hint_isLocked( firstHintEdge )            )
    {
      if ( isPair )
      {
         
         
        CF2_Fixed  midpoint  = cf2_hintmap_map(
                                 hintmap->initialHintMap,
                                 ( secondHintEdge->csCoord +
                                   firstHintEdge->csCoord ) / 2 );
        CF2_Fixed  halfWidth = FT_MulFix(
                                 ( secondHintEdge->csCoord -
                                   firstHintEdge->csCoord ) / 2,
                                 hintmap->scale );


        firstHintEdge->dsCoord  = midpoint - halfWidth;
        secondHintEdge->dsCoord = midpoint + halfWidth;
      }
      else
        firstHintEdge->dsCoord = cf2_hintmap_map( hintmap->initialHintMap,
                                                  firstHintEdge->csCoord );
    }

     

    if ( indexInsert > 0 )
    {
       
      if ( firstHintEdge->dsCoord < hintmap->edge[indexInsert - 1].dsCoord )
        return;
    }

    if ( indexInsert < hintmap->count )
    {
       
      if ( isPair )
      {
        if ( secondHintEdge->dsCoord > hintmap->edge[indexInsert].dsCoord )
          return;
      }
      else
      {
        if ( firstHintEdge->dsCoord > hintmap->edge[indexInsert].dsCoord )
          return;
      }
    }

     
    {
      CF2_Int  iSrc = hintmap->count - 1;
      CF2_Int  iDst = isPair ? hintmap->count + 1 : hintmap->count;

      CF2_Int  count = hintmap->count - indexInsert;


      if ( iDst >= CF2_MAX_HINT_EDGES )
      {
        FT_TRACE4(( "cf2_hintmap_insertHint: too many hintmaps\n" ));
        return;
      }

      while ( count-- )
        hintmap->edge[iDst--] = hintmap->edge[iSrc--];

       
      hintmap->edge[indexInsert] = *firstHintEdge;          
      hintmap->count += 1;

      if ( isPair )
      {
         
        hintmap->edge[indexInsert + 1] = *secondHintEdge;   
        hintmap->count                += 1;
      }
    }

    return;
  }
