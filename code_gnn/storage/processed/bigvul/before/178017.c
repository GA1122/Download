   tt_cmap8_validate( FT_Byte*      table,
                      FT_Validator  valid )
   {
     FT_Byte*   p = table + 4;
     FT_Byte*   is32;
     FT_UInt32  length;
     FT_UInt32  num_groups;
 
 
     if ( table + 16 + 8192 > valid->limit )
       FT_INVALID_TOO_SHORT;
 
     length = TT_NEXT_ULONG( p );
     if ( length > (FT_UInt32)( valid->limit - table ) || length < 8192 + 16 )
       FT_INVALID_TOO_SHORT;
 
     is32       = table + 12;
      p          = is32  + 8192;           
      num_groups = TT_NEXT_ULONG( p );
  
    if ( p + num_groups * 12 > valid->limit )
//      
//     if ( num_groups > (FT_UInt32)( valid->limit - p ) / 12 )
        FT_INVALID_TOO_SHORT;
  
       
       FT_UInt32  n, start, end, start_id, count, last = 0;
 
 
       for ( n = 0; n < num_groups; n++ )
       {
         FT_UInt   hi, lo;
 
 
         start    = TT_NEXT_ULONG( p );
         end      = TT_NEXT_ULONG( p );
         start_id = TT_NEXT_ULONG( p );
 
         if ( start > end )
           FT_INVALID_DATA;
 
         if ( n > 0 && start <= last )
           FT_INVALID_DATA;
 
  
          if ( valid->level >= FT_VALIDATE_TIGHT )
          {
          if ( start_id + end - start >= TT_VALID_GLYPH_COUNT( valid ) )
//           FT_UInt32  d = end - start;
// 
// 
//            
//           if ( d > TT_VALID_GLYPH_COUNT( valid )             ||
//                start_id >= TT_VALID_GLYPH_COUNT( valid ) - d )
              FT_INVALID_GLYPH_ID;
  
            count = (FT_UInt32)( end - start + 1 );
             {
               hi = (FT_UInt)( start >> 16 );
               lo = (FT_UInt)( start & 0xFFFFU );
 
               if ( (is32[hi >> 3] & ( 0x80 >> ( hi & 7 ) ) ) == 0 )
                 FT_INVALID_DATA;
 
               if ( (is32[lo >> 3] & ( 0x80 >> ( lo & 7 ) ) ) == 0 )
                 FT_INVALID_DATA;
             }
           }
           else
           {
              
              
 
              
             if ( end & ~0xFFFFU )
               FT_INVALID_DATA;
 
             for ( ; count > 0; count--, start++ )
             {
               lo = (FT_UInt)( start & 0xFFFFU );
 
               if ( (is32[lo >> 3] & ( 0x80 >> ( lo & 7 ) ) ) != 0 )
                 FT_INVALID_DATA;
             }
           }
         }
 
         last = end;
       }