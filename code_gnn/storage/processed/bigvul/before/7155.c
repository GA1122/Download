  cf2_hintmap_init( CF2_HintMap   hintmap,
                    CF2_Font      font,
                    CF2_HintMap   initialMap,
                    CF2_ArrStack  hintMoves,
                    CF2_Fixed     scale )
  {
    FT_ZERO( hintmap );

     
    hintmap->hinted         = font->hinted;
    hintmap->scale          = scale;
    hintmap->font           = font;
    hintmap->initialHintMap = initialMap;
     
    hintmap->hintMoves      = hintMoves;
  }
