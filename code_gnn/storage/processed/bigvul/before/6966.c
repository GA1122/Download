  pcf_has_table_type( PCF_Table  tables,
                      FT_ULong   ntables,  
                      FT_ULong   type )
  {
    FT_ULong  i;


    for ( i = 0; i < ntables; i++ )
      if ( tables[i].type == type )
        return TRUE;

    return FALSE;
  }