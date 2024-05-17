   cf2_hintmap_build( CF2_HintMap   hintmap,
                      CF2_ArrStack  hStemHintArray,
                      CF2_ArrStack  vStemHintArray,
                      CF2_HintMask  hintMask,
                      CF2_Fixed     hintOrigin,
                      FT_Bool       initialMap )
   {
     FT_Byte*  maskPtr;
 
     CF2_Font         font = hintmap->font;
     CF2_HintMaskRec  tempHintMask;
 
     size_t   bitCount, i;
     FT_Byte  maskByte;
 
 
      
     if ( !initialMap && !cf2_hintmap_isValid( hintmap->initialHintMap ) )
     {
        
        
       cf2_hintmask_init( &tempHintMask, hintMask->error );
       cf2_hintmap_build( hintmap->initialHintMap,
                          hStemHintArray,
                          vStemHintArray,
                          &tempHintMask,
                          hintOrigin,
                          TRUE );
     }
 
     if ( !cf2_hintmask_isValid( hintMask ) )
     {
        
       cf2_hintmask_setAll( hintMask,
                            cf2_arrstack_size( hStemHintArray ) +
                              cf2_arrstack_size( vStemHintArray ) );
       if ( !cf2_hintmask_isValid( hintMask ) )
           return;                    
     }
 
      
     hintmap->count     = 0;
     hintmap->lastIndex = 0;
 
      
     tempHintMask = *hintMask;
      maskPtr      = cf2_hintmask_getMaskPtr( &tempHintMask );
  
       
     
      bitCount = cf2_arrstack_size( hStemHintArray );
  
//      
//     if ( bitCount > hintMask->bitCount )
//         return;
// 
       
      if ( font->blues.doEmBoxHints )
      {
       cf2_hint_initZero( &dummy );    
 
        
       cf2_hintmap_insertHint( hintmap,
                               &font->blues.emBoxBottomEdge,
                               &dummy );
        
       cf2_hintmap_insertHint( hintmap,
                               &dummy,
                               &font->blues.emBoxTopEdge );
     }
 
      
      
     for ( i = 0, maskByte = 0x80; i < bitCount; i++ )
     {
       if ( maskByte & *maskPtr )
       {
          
         CF2_HintRec  bottomHintEdge, topHintEdge;
 
 
         cf2_hint_init( &bottomHintEdge,
                        hStemHintArray,
                        i,
                        font,
                        hintOrigin,
                        hintmap->scale,
                        TRUE   );
         cf2_hint_init( &topHintEdge,
                        hStemHintArray,
                        i,
                        font,
                        hintOrigin,
                        hintmap->scale,
                        FALSE   );
 
         if ( cf2_hint_isLocked( &bottomHintEdge ) ||
              cf2_hint_isLocked( &topHintEdge )    ||
              cf2_blues_capture( &font->blues,
                                 &bottomHintEdge,
                                 &topHintEdge )   )
         {
            
           cf2_hintmap_insertHint( hintmap, &bottomHintEdge, &topHintEdge );
 
           *maskPtr &= ~maskByte;       
         }
       }
 
       if ( ( i & 7 ) == 7 )
       {
          
         maskPtr++;
         maskByte = 0x80;
       }
       else
         maskByte >>= 1;
     }
 
      
 
      
 
     if ( initialMap )
     {
        
        
        
 
       if ( hintmap->count == 0                           ||
            hintmap->edge[0].csCoord > 0                  ||
            hintmap->edge[hintmap->count - 1].csCoord < 0 )
       {
          
          
 
         CF2_HintRec  edge, invalid;
 
 
         cf2_hint_initZero( &edge );
 
         edge.flags = CF2_GhostBottom |
                      CF2_Locked      |
                      CF2_Synthetic;
         edge.scale = hintmap->scale;
 
         cf2_hint_initZero( &invalid );
         cf2_hintmap_insertHint( hintmap, &edge, &invalid );
       }
     }
     else
     {
        
 
       maskPtr = cf2_hintmask_getMaskPtr( &tempHintMask );
 
       for ( i = 0, maskByte = 0x80; i < bitCount; i++ )
       {
         if ( maskByte & *maskPtr )
         {
           CF2_HintRec  bottomHintEdge, topHintEdge;
 
 
           cf2_hint_init( &bottomHintEdge,
                          hStemHintArray,
                          i,
                          font,
                          hintOrigin,
                          hintmap->scale,
                          TRUE   );
           cf2_hint_init( &topHintEdge,
                          hStemHintArray,
                          i,
                          font,
                          hintOrigin,
                          hintmap->scale,
                          FALSE   );
 
           cf2_hintmap_insertHint( hintmap, &bottomHintEdge, &topHintEdge );
         }
 
         if ( ( i & 7 ) == 7 )
         {
            
           maskPtr++;
           maskByte = 0x80;
         }
         else
           maskByte >>= 1;
       }
     }
 
      
 
      
     cf2_hintmap_adjustHints( hintmap );
 
      
      
     if ( !initialMap )
     {
       for ( i = 0; i < hintmap->count; i++ )
       {
         if ( !cf2_hint_isSynthetic( &hintmap->edge[i] ) )
         {
            
            
           CF2_StemHint  stemhint = (CF2_StemHint)
                           cf2_arrstack_getPointer( hStemHintArray,
                                                    hintmap->edge[i].index );
 
 
           if ( cf2_hint_isTop( &hintmap->edge[i] ) )
             stemhint->maxDS = hintmap->edge[i].dsCoord;
           else
             stemhint->minDS = hintmap->edge[i].dsCoord;
 
           stemhint->used = TRUE;
         }
       }
     }
 
      
     hintmap->isValid = TRUE;
 
      
     cf2_hintmask_setNew( hintMask, FALSE );
   }