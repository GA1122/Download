   cff_charset_load( CFF_Charset  charset,
                     FT_UInt      num_glyphs,
                     FT_Stream    stream,
                     FT_ULong     base_offset,
                     FT_ULong     offset,
                     FT_Bool      invert )
   {
     FT_Memory  memory = stream->memory;
     FT_Error   error  = CFF_Err_Ok;
     FT_UShort  glyph_sid;
 
 
      
      
     if ( offset > 2 )
     {
       FT_UInt  j;
 
 
       charset->offset = base_offset + offset;
 
        
       if ( FT_STREAM_SEEK( charset->offset ) ||
            FT_READ_BYTE( charset->format )   )
         goto Exit;
 
        
       if ( FT_NEW_ARRAY( charset->sids, num_glyphs ) )
         goto Exit;
 
        
       charset->sids[0] = 0;
 
       switch ( charset->format )
       {
       case 0:
         if ( num_glyphs > 0 )
         {
           if ( FT_FRAME_ENTER( ( num_glyphs - 1 ) * 2 ) )
              goto Exit;
  
            for ( j = 1; j < num_glyphs; j++ )
            charset->sids[j] = FT_GET_USHORT();
//           {
//             FT_UShort sid = FT_GET_USHORT();
// 
// 
//              
//             if ( sid < 65000 )
//               charset->sids[j] = sid;
//             else
//             {
//               FT_ERROR(( "cff_charset_load:"
//                          " invalid SID value %d set to zero\n", sid ));
//               charset->sids[j] = 0;
//             }
//           }
  
            FT_FRAME_EXIT();
          }
              
             if ( FT_READ_USHORT( glyph_sid ) )
               goto Exit;
 
              
             if ( charset->format == 2 )
             {
               if ( FT_READ_USHORT( nleft ) )
                 goto Exit;
             }
             else
             {
               if ( FT_READ_BYTE( nleft ) )
                 goto Exit;
             }
 
              
             for ( i = 0; j < num_glyphs && i <= nleft; i++, j++, glyph_sid++ )
               charset->sids[j] = glyph_sid;
           }
         }
         break;
 
       default:
         FT_ERROR(( "cff_charset_load: invalid table format!\n" ));
         error = CFF_Err_Invalid_File_Format;
                  goto Exit;
              }