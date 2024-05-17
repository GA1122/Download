static HB_Error  Lookup_PairPos( GPOS_Instance*    gpi,
				 HB_GPOS_SubTable* st,
				 HB_Buffer        buffer,
				 HB_UShort         flags,
				 HB_UShort         context_length,
				 int               nesting_level )
{
  HB_Error         error;
  HB_UShort        index, property;
  HB_UInt          first_pos;
  HB_GPOSHeader*  gpos = gpi->gpos;
  HB_PairPos*     pp = &st->pair;

  HB_UNUSED(nesting_level);

  if ( buffer->in_pos >= buffer->in_length - 1 )
    return HB_Err_Not_Covered;            

  if ( context_length != 0xFFFF && context_length < 2 )
    return HB_Err_Not_Covered;

  if ( CHECK_Property( gpos->gdef, IN_CURITEM(), flags, &property ) )
    return error;

  error = _HB_OPEN_Coverage_Index( &pp->Coverage, IN_CURGLYPH(), &index );
  if ( error )
    return error;

   

  first_pos = buffer->in_pos;
  (buffer->in_pos)++;

  while ( CHECK_Property( gpos->gdef, IN_CURITEM(),
			  flags, &property ) )
  {
    if ( error && error != HB_Err_Not_Covered )
      return error;

    if ( buffer->in_pos == buffer->in_length )
      {
	buffer->in_pos = first_pos;
        return HB_Err_Not_Covered;
      }
    (buffer->in_pos)++;

  }

  switch ( pp->PosFormat )
  {
  case 1:
    error = Lookup_PairPos1( gpi, &pp->ppf.ppf1, buffer,
			     first_pos, index,
			     pp->ValueFormat1, pp->ValueFormat2 );
    break;

  case 2:
    error = Lookup_PairPos2( gpi, &pp->ppf.ppf2, buffer, first_pos,
			     pp->ValueFormat1, pp->ValueFormat2 );
    break;

  default:
    return ERR(HB_Err_Invalid_SubTable_Format);
  }

   
  if ( error == HB_Err_Not_Covered )
      buffer->in_pos = first_pos;

   

  if ( pp->ValueFormat2 )
    (buffer->in_pos)++;

  return error;
}
