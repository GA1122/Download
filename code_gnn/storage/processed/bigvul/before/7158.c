  cf2_hintmap_map( CF2_HintMap  hintmap,
                   CF2_Fixed    csCoord )
  {
    FT_ASSERT( hintmap->isValid );   
    FT_ASSERT( hintmap->lastIndex < CF2_MAX_HINT_EDGES );

    if ( hintmap->count == 0 || ! hintmap->hinted )
    {
       
      return FT_MulFix( csCoord, hintmap->scale );
    }
    else
    {
       
      CF2_UInt  i = hintmap->lastIndex;


       
      while ( i < hintmap->count - 1                  &&
              csCoord >= hintmap->edge[i + 1].csCoord )
        i += 1;

       
      while ( i > 0 && csCoord < hintmap->edge[i].csCoord )
        i -= 1;

      hintmap->lastIndex = i;

      if ( i == 0 && csCoord < hintmap->edge[0].csCoord )
      {
         
        return FT_MulFix( csCoord - hintmap->edge[0].csCoord,
                          hintmap->scale ) +
                 hintmap->edge[0].dsCoord;
      }
      else
      {
         
        return FT_MulFix( csCoord - hintmap->edge[i].csCoord,
                          hintmap->edge[i].scale ) +
                 hintmap->edge[i].dsCoord;
      }
    }
  }
