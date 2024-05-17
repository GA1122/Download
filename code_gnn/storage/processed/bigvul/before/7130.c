  _bdf_list_ensure( _bdf_list_t*   list,
                    unsigned long  num_items )  
  {
    FT_Error  error = FT_Err_Ok;


    if ( num_items > list->size )
    {
      unsigned long  oldsize = list->size;  
      unsigned long  newsize = oldsize + ( oldsize >> 1 ) + 5;
      unsigned long  bigsize = (unsigned long)( FT_INT_MAX / sizeof ( char* ) );
      FT_Memory      memory  = list->memory;


      if ( oldsize == bigsize )
      {
        error = FT_THROW( Out_Of_Memory );
        goto Exit;
      }
      else if ( newsize < oldsize || newsize > bigsize )
        newsize = bigsize;

      if ( FT_RENEW_ARRAY( list->field, oldsize, newsize ) )
        goto Exit;

      list->size = newsize;
    }

  Exit:
    return error;
  }
