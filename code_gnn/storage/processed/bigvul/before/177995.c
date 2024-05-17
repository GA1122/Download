   _bdf_parse_glyphs( char*          line,
                      unsigned long  linelen,
                      unsigned long  lineno,
                      void*          call_data,
                      void*          client_data )
   {
     int                c, mask_index;
     char*              s;
     unsigned char*     bp;
     unsigned long      i, slen, nibbles;
 
     _bdf_parse_t*      p;
     bdf_glyph_t*       glyph;
     bdf_font_t*        font;
 
     FT_Memory          memory;
     FT_Error           error = BDF_Err_Ok;
 
     FT_UNUSED( call_data );
     FT_UNUSED( lineno );         
 
 
     p = (_bdf_parse_t *)client_data;
 
     font   = p->font;
     memory = font->memory;
 
      
     if ( ft_memcmp( line, "COMMENT", 7 ) == 0 )
     {
       linelen -= 7;
 
       s = line + 7;
       if ( *s != 0 )
       {
         s++;
         linelen--;
       }
       error = _bdf_add_comment( p->font, s, linelen );
       goto Exit;
     }
 
      
     if ( !( p->flags & _BDF_GLYPHS ) )
     {
       if ( ft_memcmp( line, "CHARS", 5 ) != 0 )
       {
         FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG1, lineno, "CHARS" ));
         error = BDF_Err_Missing_Chars_Field;
         goto Exit;
       }
 
       error = _bdf_list_split( &p->list, (char *)" +", line, linelen );
       if ( error )
         goto Exit;
       p->cnt = font->glyphs_size = _bdf_atoul( p->list.field[1], 0, 10 );
 
        
       if ( p->cnt == 0 )
         font->glyphs_size = 64;
 
        
        
       if ( p->cnt >= 0x110000UL )
       {
         FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG5, lineno, "CHARS" ));
         error = BDF_Err_Invalid_Argument;
         goto Exit;
       }
 
       if ( FT_NEW_ARRAY( font->glyphs, font->glyphs_size ) )
         goto Exit;
 
       p->flags |= _BDF_GLYPHS;
 
       goto Exit;
     }
 
      
     if ( ft_memcmp( line, "ENDFONT", 7 ) == 0 )
     {
        
       ft_qsort( (char *)font->glyphs,
                 font->glyphs_used,
                 sizeof ( bdf_glyph_t ),
                 by_encoding );
 
       p->flags &= ~_BDF_START;
 
       goto Exit;
     }
 
      
     if ( ft_memcmp( line, "ENDCHAR", 7 ) == 0 )
     {
       p->glyph_enc = 0;
       p->flags    &= ~_BDF_GLYPH_BITS;
 
       goto Exit;
     }
 
      
      
     if ( ( p->flags & _BDF_GLYPH )     &&
          p->glyph_enc            == -1 &&
          p->opts->keep_unencoded == 0  )
       goto Exit;
 
      
     if ( ft_memcmp( line, "STARTCHAR", 9 ) == 0 )
     {
        
        
       FT_FREE( p->glyph_name );
 
       error = _bdf_list_split( &p->list, (char *)" +", line, linelen );
       if ( error )
         goto Exit;
 
       _bdf_list_shift( &p->list, 1 );
 
       s = _bdf_list_join( &p->list, ' ', &slen );
 
       if ( !s )
       {
         FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG8, lineno, "STARTCHAR" ));
         error = BDF_Err_Invalid_File_Format;
         goto Exit;
       }
 
       if ( FT_NEW_ARRAY( p->glyph_name, slen + 1 ) )
         goto Exit;
 
       FT_MEM_COPY( p->glyph_name, s, slen + 1 );
 
       p->flags |= _BDF_GLYPH;
 
       FT_TRACE4(( DBGMSG1, lineno, s ));
 
       goto Exit;
     }
 
      
     if ( ft_memcmp( line, "ENCODING", 8 ) == 0 )
     {
       if ( !( p->flags & _BDF_GLYPH ) )
       {
          
         FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG1, lineno, "STARTCHAR" ));
         error = BDF_Err_Missing_Startchar_Field;
         goto Exit;
       }
 
       error = _bdf_list_split( &p->list, (char *)" +", line, linelen );
       if ( error )
         goto Exit;
 
       p->glyph_enc = _bdf_atol( p->list.field[1], 0, 10 );
 
        
        
       if ( p->glyph_enc < -1 )
         p->glyph_enc = -1;
 
        
       if ( p->glyph_enc == -1 && p->list.used > 2 )
         p->glyph_enc = _bdf_atol( p->list.field[2], 0, 10 );
 
       FT_TRACE4(( DBGMSG2, p->glyph_enc ));
  
         
         
      if ( p->glyph_enc > 0                               &&
           (size_t)p->glyph_enc >= sizeof ( p->have ) * 8 )
//       if ( p->glyph_enc > 0                                      &&
//            (size_t)p->glyph_enc >= sizeof ( p->have ) /
//                                    sizeof ( unsigned long ) * 32 )
        {
          FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG5, lineno, "ENCODING" ));
          error = BDF_Err_Invalid_File_Format;
       }
 
        
        
        
       if ( p->glyph_enc >= 0 )
       {
         if ( _bdf_glyph_modified( p->have, p->glyph_enc ) )
         {
            
            
           FT_TRACE2(( "_bdf_parse_glyphs: " ACMSG12,
                       p->glyph_enc, p->glyph_name ));
           p->glyph_enc = -1;
           font->modified = 1;
         }
         else
           _bdf_set_glyph_modified( p->have, p->glyph_enc );
       }
 
       if ( p->glyph_enc >= 0 )
       {
          
          
         if ( font->glyphs_used == font->glyphs_size )
         {
           if ( FT_RENEW_ARRAY( font->glyphs,
                                font->glyphs_size,
                                font->glyphs_size + 64 ) )
             goto Exit;
 
           font->glyphs_size += 64;
         }
 
         glyph           = font->glyphs + font->glyphs_used++;
         glyph->name     = p->glyph_name;
         glyph->encoding = p->glyph_enc;
 
          
         p->glyph_name = 0;
       }
       else
       {
          
          
         if ( p->opts->keep_unencoded != 0 )
         {
            
           if ( font->unencoded_used == font->unencoded_size )
           {
             if ( FT_RENEW_ARRAY( font->unencoded ,
                                  font->unencoded_size,
                                  font->unencoded_size + 4 ) )
               goto Exit;
 
             font->unencoded_size += 4;
           }
 
           glyph           = font->unencoded + font->unencoded_used;
           glyph->name     = p->glyph_name;
           glyph->encoding = font->unencoded_used++;
         }
         else
            
            
           FT_FREE( p->glyph_name );
 
         p->glyph_name = 0;
       }
 
        
        
       p->flags &= ~( _BDF_GLYPH_WIDTH_CHECK | _BDF_GLYPH_HEIGHT_CHECK );
 
       p->flags |= _BDF_ENCODING;
 
       goto Exit;
     }
 
      
     if ( p->glyph_enc == -1 )
       glyph = font->unencoded + ( font->unencoded_used - 1 );
     else
       glyph = font->glyphs + ( font->glyphs_used - 1 );
 
      
     if ( p->flags & _BDF_BITMAP )
     {
        
        
       if ( p->row >= (unsigned long)glyph->bbx.height )
       {
         if ( !( p->flags & _BDF_GLYPH_HEIGHT_CHECK ) )
         {
           FT_TRACE2(( "_bdf_parse_glyphs: " ACMSG13, glyph->encoding ));
           p->flags |= _BDF_GLYPH_HEIGHT_CHECK;
           font->modified = 1;
         }
 
         goto Exit;
       }
 
        
        
       nibbles = glyph->bpr << 1;
       bp      = glyph->bitmap + p->row * glyph->bpr;
 
       for ( i = 0; i < nibbles; i++ )
       {
         c = line[i];
         if ( !sbitset( hdigits, c ) )
           break;
         *bp = (FT_Byte)( ( *bp << 4 ) + a2i[c] );
         if ( i + 1 < nibbles && ( i & 1 ) )
           *++bp = 0;
       }
 
        
        
       if ( i < nibbles                            &&
            !( p->flags & _BDF_GLYPH_WIDTH_CHECK ) )
       {
         FT_TRACE2(( "_bdf_parse_glyphs: " ACMSG16, glyph->encoding ));
         p->flags       |= _BDF_GLYPH_WIDTH_CHECK;
         font->modified  = 1;
       }
 
        
       mask_index = ( glyph->bbx.width * p->font->bpp ) & 7;
       if ( glyph->bbx.width )
         *bp &= nibble_mask[mask_index];
 
        
       if ( i == nibbles                           &&
            sbitset( hdigits, line[nibbles] )      &&
            !( p->flags & _BDF_GLYPH_WIDTH_CHECK ) )
       {
         FT_TRACE2(( "_bdf_parse_glyphs: " ACMSG14, glyph->encoding ));
         p->flags       |= _BDF_GLYPH_WIDTH_CHECK;
         font->modified  = 1;
       }
 
       p->row++;
       goto Exit;
     }
 
      
     if ( ft_memcmp( line, "SWIDTH", 6 ) == 0 )
     {
       if ( !( p->flags & _BDF_ENCODING ) )
         goto Missing_Encoding;
 
       error = _bdf_list_split( &p->list, (char *)" +", line, linelen );
       if ( error )
         goto Exit;
 
       glyph->swidth = (unsigned short)_bdf_atoul( p->list.field[1], 0, 10 );
       p->flags |= _BDF_SWIDTH;
 
       goto Exit;
     }
 
      
     if ( ft_memcmp( line, "DWIDTH", 6 ) == 0 )
     {
       if ( !( p->flags & _BDF_ENCODING ) )
         goto Missing_Encoding;
 
       error = _bdf_list_split( &p->list, (char *)" +", line, linelen );
       if ( error )
         goto Exit;
 
       glyph->dwidth = (unsigned short)_bdf_atoul( p->list.field[1], 0, 10 );
 
       if ( !( p->flags & _BDF_SWIDTH ) )
       {
          
          
         FT_TRACE2(( "_bdf_parse_glyphs: " ACMSG9, lineno ));
 
         glyph->swidth = (unsigned short)FT_MulDiv(
                           glyph->dwidth, 72000L,
                           (FT_Long)( font->point_size *
                                      font->resolution_x ) );
       }
 
       p->flags |= _BDF_DWIDTH;
       goto Exit;
     }
 
      
     if ( ft_memcmp( line, "BBX", 3 ) == 0 )
     {
       if ( !( p->flags & _BDF_ENCODING ) )
         goto Missing_Encoding;
 
       error = _bdf_list_split( &p->list, (char *)" +", line, linelen );
       if ( error )
         goto Exit;
 
       glyph->bbx.width    = _bdf_atos( p->list.field[1], 0, 10 );
       glyph->bbx.height   = _bdf_atos( p->list.field[2], 0, 10 );
       glyph->bbx.x_offset = _bdf_atos( p->list.field[3], 0, 10 );
       glyph->bbx.y_offset = _bdf_atos( p->list.field[4], 0, 10 );
 
        
       glyph->bbx.ascent  = (short)( glyph->bbx.height + glyph->bbx.y_offset );
       glyph->bbx.descent = (short)( -glyph->bbx.y_offset );
 
        
        
       p->maxas    = (short)FT_MAX( glyph->bbx.ascent, p->maxas );
       p->maxds    = (short)FT_MAX( glyph->bbx.descent, p->maxds );
 
       p->rbearing = (short)( glyph->bbx.width + glyph->bbx.x_offset );
 
       p->maxrb    = (short)FT_MAX( p->rbearing, p->maxrb );
       p->minlb    = (short)FT_MIN( glyph->bbx.x_offset, p->minlb );
       p->maxlb    = (short)FT_MAX( glyph->bbx.x_offset, p->maxlb );
 
       if ( !( p->flags & _BDF_DWIDTH ) )
       {
          
          
         FT_TRACE2(( "_bdf_parse_glyphs: " ACMSG10, lineno ));
         glyph->dwidth = glyph->bbx.width;
       }
 
        
        
       if ( p->opts->correct_metrics != 0 )
       {
          
         unsigned short  sw = (unsigned short)FT_MulDiv(
                                glyph->dwidth, 72000L,
                                (FT_Long)( font->point_size *
                                           font->resolution_x ) );
 
 
         if ( sw != glyph->swidth )
         {
           glyph->swidth = sw;
 
           if ( p->glyph_enc == -1 )
             _bdf_set_glyph_modified( font->umod,
                                      font->unencoded_used - 1 );
           else
             _bdf_set_glyph_modified( font->nmod, glyph->encoding );
 
           p->flags       |= _BDF_SWIDTH_ADJ;
           font->modified  = 1;
         }
       }
 
       p->flags |= _BDF_BBX;
       goto Exit;
     }
 
      
     if ( ft_memcmp( line, "BITMAP", 6 ) == 0 )
     {
       unsigned long  bitmap_size;
 
 
       if ( !( p->flags & _BDF_BBX ) )
       {
          
         FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG1, lineno, "BBX" ));
         error = BDF_Err_Missing_Bbx_Field;
         goto Exit;
       }
 
        
       glyph->bpr = ( glyph->bbx.width * p->font->bpp + 7 ) >> 3;
 
       bitmap_size = glyph->bpr * glyph->bbx.height;
       if ( glyph->bpr > 0xFFFFU || bitmap_size > 0xFFFFU )
       {
         FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG4, lineno ));
         error = BDF_Err_Bbx_Too_Big;
         goto Exit;
       }
       else
         glyph->bytes = (unsigned short)bitmap_size;
 
       if ( FT_NEW_ARRAY( glyph->bitmap, glyph->bytes ) )
         goto Exit;
 
       p->row    = 0;
       p->flags |= _BDF_BITMAP;
 
       goto Exit;
     }
 
     FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG9, lineno ));
     error = BDF_Err_Invalid_File_Format;
     goto Exit;
 
   Missing_Encoding:
      
     FT_ERROR(( "_bdf_parse_glyphs: " ERRMSG1, lineno, "ENCODING" ));
     error = BDF_Err_Missing_Encoding_Field;
 
   Exit:
     if ( error && ( p->flags & _BDF_GLYPH ) )
       FT_FREE( p->glyph_name );
 
     return error;
   }