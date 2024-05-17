static HB_Error  GPOS_Do_String_Lookup( GPOS_Instance*    gpi,
				   HB_UShort         lookup_index,
				   HB_Buffer        buffer )
{
  HB_Error         error, retError = HB_Err_Not_Covered;
  HB_GPOSHeader*  gpos = gpi->gpos;

  HB_UInt*  properties = gpos->LookupList.Properties;

  const int       nesting_level = 0;
   
  const HB_UShort context_length = 0xFFFF;


  gpi->last  = 0xFFFF;      

  buffer->in_pos = 0;
  while ( buffer->in_pos < buffer->in_length )
  {
    if ( ~IN_PROPERTIES( buffer->in_pos ) & properties[lookup_index] )
    {
       

      error = GPOS_Do_Glyph_Lookup( gpi, lookup_index, buffer, context_length, nesting_level );
      if ( error && error != HB_Err_Not_Covered )
	return error;
    }
    else
    {
       
      gpi->last = 0xFFFF;

      error = HB_Err_Not_Covered;
    }

    if ( error == HB_Err_Not_Covered )
      (buffer->in_pos)++;
    else
      retError = error;
  }

  return retError;
}
