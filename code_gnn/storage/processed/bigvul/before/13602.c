static HB_Error  Lookup_ContextPos1( GPOS_Instance*          gpi,
				     HB_ContextPosFormat1*  cpf1,
				     HB_Buffer              buffer,
				     HB_UShort               flags,
				     HB_UShort               context_length,
				     int                     nesting_level )
{
  HB_UShort        index, property;
  HB_UShort        i, j, k, numpr;
  HB_Error         error;
  HB_GPOSHeader*  gpos = gpi->gpos;

  HB_PosRule*     pr;
  HB_GDEFHeader*  gdef;


  gdef = gpos->gdef;

  if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
    return error;

  error = _HB_OPEN_Coverage_Index( &cpf1->Coverage, IN_CURGLYPH(), &index );
  if ( error )
    return error;

  pr    = cpf1->PosRuleSet[index].PosRule;
  numpr = cpf1->PosRuleSet[index].PosRuleCount;

  for ( k = 0; k < numpr; k++ )
  {
    if ( context_length != 0xFFFF && context_length < pr[k].GlyphCount )
      goto next_posrule;

    if ( buffer->in_pos + pr[k].GlyphCount > buffer->in_length )
      goto next_posrule;                        

    for ( i = 1, j = buffer->in_pos + 1; i < pr[k].GlyphCount; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
	if ( error && error != HB_Err_Not_Covered )
	  return error;

	if ( j + pr[k].GlyphCount - i == (HB_Int)buffer->in_length )
	  goto next_posrule;
	j++;
      }

      if ( IN_GLYPH( j ) != pr[k].Input[i - 1] )
	goto next_posrule;
    }

    return Do_ContextPos( gpi, pr[k].GlyphCount,
			  pr[k].PosCount, pr[k].PosLookupRecord,
			  buffer,
			  nesting_level );

    next_posrule:
      ;
  }

  return HB_Err_Not_Covered;
}
