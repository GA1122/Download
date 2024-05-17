  ftc_snode_compare( FTC_Node    ftcsnode,
                     FT_Pointer  ftcgquery,
                     FTC_Cache   cache,
                     FT_Bool*    list_changed )
  {
    FTC_SNode   snode  = (FTC_SNode)ftcsnode;
    FTC_GQuery  gquery = (FTC_GQuery)ftcgquery;
    FTC_GNode   gnode  = FTC_GNODE( snode );
    FT_UInt     gindex = gquery->gindex;
    FT_Bool     result;


    if (list_changed)
      *list_changed = FALSE;
    result = FT_BOOL( gnode->family == gquery->family                    &&
                      (FT_UInt)( gindex - gnode->gindex ) < snode->count );
    if ( result )
    {
       
      FTC_SBit  sbit = snode->sbits + ( gindex - gnode->gindex );


       

      if ( sbit->buffer == NULL && sbit->width == 255 )
      {
        FT_ULong  size;
        FT_Error  error;


        ftcsnode->ref_count++;   
                                 

        FTC_CACHE_TRYLOOP( cache )
        {
          error = ftc_snode_load( snode, cache->manager, gindex, &size );
        }
        FTC_CACHE_TRYLOOP_END( list_changed );

        ftcsnode->ref_count--;   

        if ( error )
          result = 0;
        else
          cache->manager->cur_weight += size;
      }
    }

    return result;
  }
