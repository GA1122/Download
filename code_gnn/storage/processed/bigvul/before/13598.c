static HB_Error  Lookup_ChainContextPos1(
		   GPOS_Instance*               gpi,
		   HB_ChainContextPosFormat1*  ccpf1,
		   HB_Buffer                   buffer,
		   HB_UShort                    flags,
		   HB_UShort                    context_length,
		   int                          nesting_level )
{
  HB_UShort          index, property;
  HB_UShort          i, j, k, num_cpr;
  HB_UShort          bgc, igc, lgc;
  HB_Error           error;
  HB_GPOSHeader*    gpos = gpi->gpos;

  HB_ChainPosRule*  cpr;
  HB_ChainPosRule   curr_cpr;
  HB_GDEFHeader*    gdef;


  gdef = gpos->gdef;

  if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
    return error;

  error = _HB_OPEN_Coverage_Index( &ccpf1->Coverage, IN_CURGLYPH(), &index );
  if ( error )
    return error;

  cpr     = ccpf1->ChainPosRuleSet[index].ChainPosRule;
  num_cpr = ccpf1->ChainPosRuleSet[index].ChainPosRuleCount;

  for ( k = 0; k < num_cpr; k++ )
  {
    curr_cpr = cpr[k];
    bgc      = curr_cpr.BacktrackGlyphCount;
    igc      = curr_cpr.InputGlyphCount;
    lgc      = curr_cpr.LookaheadGlyphCount;

    if ( context_length != 0xFFFF && context_length < igc )
      goto next_chainposrule;

     

    if ( bgc > buffer->in_pos || buffer->in_pos + igc + lgc > buffer->in_length )
      goto next_chainposrule;

    if ( bgc )
    {
       

      for ( i = 0, j = buffer->in_pos - 1; i < bgc; i++, j-- )
      {
	while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
	{
	  if ( error && error != HB_Err_Not_Covered )
	    return error;

	  if ( j + 1 == bgc - i )
	    goto next_chainposrule;
	  j--;
	}

	 

	if ( IN_GLYPH( j ) != curr_cpr.Backtrack[i] )
	  goto next_chainposrule;
      }
    }

     

    for ( i = 1, j = buffer->in_pos + 1; i < igc; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
	if ( error && error != HB_Err_Not_Covered )
	  return error;

	if ( j + igc - i + lgc == (HB_Int)buffer->in_length )
	  goto next_chainposrule;
	j++;
      }

      if ( IN_GLYPH( j ) != curr_cpr.Input[i - 1] )
	goto next_chainposrule;
    }

     

    for ( i = 0; i < lgc; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
	if ( error && error != HB_Err_Not_Covered )
	  return error;

	if ( j + lgc - i == (HB_Int)buffer->in_length )
	  goto next_chainposrule;
	j++;
      }

      if ( IN_GLYPH( j ) != curr_cpr.Lookahead[i] )
	goto next_chainposrule;
    }

    return Do_ContextPos( gpi, igc,
			  curr_cpr.PosCount,
			  curr_cpr.PosLookupRecord,
			  buffer,
			  nesting_level );

  next_chainposrule:
    ;
  }

  return HB_Err_Not_Covered;
}