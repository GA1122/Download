   tt_size_reset( TT_Size  size,
                  FT_Bool  only_height )
   {
     TT_Face           face;
      FT_Size_Metrics*  metrics;
  
  
    size->ttmetrics.valid = FALSE;
      face = (TT_Face)size->root.face;
  
//      
//     if ( face->isCFF2 )
//       return FT_Err_Ok;
// 
//     size->ttmetrics.valid = FALSE;
// 
      metrics = &size->metrics;
  
       
 
      
      
      
      
     if ( face->header.Flags & 8 )
     {
       metrics->ascender =
         FT_PIX_ROUND( FT_MulFix( face->root.ascender, metrics->y_scale ) );
       metrics->descender =
         FT_PIX_ROUND( FT_MulFix( face->root.descender, metrics->y_scale ) );
       metrics->height =
         FT_PIX_ROUND( FT_MulFix( face->root.height, metrics->y_scale ) );
     }
 
     size->ttmetrics.valid = TRUE;
 
     if ( only_height )
       return FT_Err_Ok;
 
     if ( face->header.Flags & 8 )
     {
       metrics->x_scale = FT_DivFix( metrics->x_ppem << 6,
                                     face->root.units_per_EM );
       metrics->y_scale = FT_DivFix( metrics->y_ppem << 6,
                                     face->root.units_per_EM );
 
       metrics->max_advance =
         FT_PIX_ROUND( FT_MulFix( face->root.max_advance_width,
                                  metrics->x_scale ) );
     }
 
      
     if ( metrics->x_ppem >= metrics->y_ppem )
     {
       size->ttmetrics.scale   = metrics->x_scale;
       size->ttmetrics.ppem    = metrics->x_ppem;
       size->ttmetrics.x_ratio = 0x10000L;
       size->ttmetrics.y_ratio = FT_DivFix( metrics->y_ppem,
                                            metrics->x_ppem );
     }
     else
     {
       size->ttmetrics.scale   = metrics->y_scale;
       size->ttmetrics.ppem    = metrics->y_ppem;
       size->ttmetrics.x_ratio = FT_DivFix( metrics->x_ppem,
                                            metrics->y_ppem );
       size->ttmetrics.y_ratio = 0x10000L;
     }
 
 #ifdef TT_USE_BYTECODE_INTERPRETER
     size->cvt_ready = -1;
 #endif  
 
     return FT_Err_Ok;
   }