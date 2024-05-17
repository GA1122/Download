  psh_hint_table_init( PSH_Hint_Table  table,
                       PS_Hint_Table   hints,
                       PS_Mask_Table   hint_masks,
                       PS_Mask_Table   counter_masks,
                       FT_Memory       memory )
  {
    FT_UInt   count;
    FT_Error  error;

    FT_UNUSED( counter_masks );


    count = hints->num_hints;

     
    if ( FT_NEW_ARRAY( table->sort,  2 * count     ) ||
         FT_NEW_ARRAY( table->hints,     count     ) ||
         FT_NEW_ARRAY( table->zones, 2 * count + 1 ) )
      goto Exit;

    table->max_hints   = count;
    table->sort_global = table->sort + count;
    table->num_hints   = 0;
    table->num_zones   = 0;
    table->zone        = 0;

     
    {
      PSH_Hint  write = table->hints;
      PS_Hint   read  = hints->hints;


      for ( ; count > 0; count--, write++, read++ )
      {
        write->org_pos = read->pos;
        write->org_len = read->len;
        write->flags   = read->flags;
      }
    }

     
     
    if ( hint_masks )
    {
      PS_Mask  mask = hint_masks->masks;


      count             = hint_masks->num_masks;
      table->hint_masks = hint_masks;

      for ( ; count > 0; count--, mask++ )
        psh_hint_table_record_mask( table, mask );
    }

     
    if ( table->num_hints != table->max_hints )
    {
      FT_UInt  idx;


      FT_TRACE0(( "psh_hint_table_init: missing/incorrect hint masks\n" ));

      count = table->max_hints;
      for ( idx = 0; idx < count; idx++ )
        psh_hint_table_record( table, idx );
    }

  Exit:
    return error;
  }
