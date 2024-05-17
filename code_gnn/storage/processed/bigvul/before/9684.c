  find_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  cur;


     
    FT_ASSERT( face );

    first = face->charmaps;

    if ( !first )
      return FT_Err_Invalid_CharMap_Handle;

     

     
     
     
     

    cur = first + face->num_charmaps;   

    for ( ; --cur >= first; )
    {
      if ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
         
         
        if ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) )
        {
#ifdef FT_MAX_CHARMAP_CACHEABLE
          if ( cur - first > FT_MAX_CHARMAP_CACHEABLE )
          {
            FT_ERROR(( "find_unicode_charmap: UCS-4 cmap is found "
                       "at too late position (%d)\n", cur - first ));
            continue;
          }
#endif
          face->charmap = cur[0];
          return FT_Err_Ok;
        }
      }
    }

     
     
    cur = first + face->num_charmaps;

    for ( ; --cur >= first; )
    {
      if ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
#ifdef FT_MAX_CHARMAP_CACHEABLE
        if ( cur - first > FT_MAX_CHARMAP_CACHEABLE )
        {
          FT_ERROR(( "find_unicode_charmap: UCS-2 cmap is found "
                     "at too late position (%d)\n", cur - first ));
          continue;
        }
#endif
        face->charmap = cur[0];
        return FT_Err_Ok;
      }
    }

    return FT_Err_Invalid_CharMap_Handle;
  }
