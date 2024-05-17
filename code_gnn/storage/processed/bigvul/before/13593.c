static HB_Error  Load_PosRule( HB_PosRule*  pr,
			       HB_Stream     stream )
{
  HB_Error  error;

  HB_UShort             n, count;
  HB_UShort*            i;

  HB_PosLookupRecord*  plr;


  if ( ACCESS_Frame( 4L ) )
    return error;

  pr->GlyphCount = GET_UShort();
  pr->PosCount   = GET_UShort();

  FORGET_Frame();

  pr->Input = NULL;

  count = pr->GlyphCount - 1;          

  if ( ALLOC_ARRAY( pr->Input, count, HB_UShort ) )
    return error;

  i = pr->Input;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail2;

  for ( n = 0; n < count; n++ )
    i[n] = GET_UShort();

  FORGET_Frame();

  pr->PosLookupRecord = NULL;

  count = pr->PosCount;

  if ( ALLOC_ARRAY( pr->PosLookupRecord, count, HB_PosLookupRecord ) )
    goto Fail2;

  plr = pr->PosLookupRecord;

  if ( ACCESS_Frame( count * 4L ) )
    goto Fail1;

  for ( n = 0; n < count; n++ )
  {
    plr[n].SequenceIndex   = GET_UShort();
    plr[n].LookupListIndex = GET_UShort();
  }

  FORGET_Frame();

  return HB_Err_Ok;

Fail1:
  FREE( plr );

Fail2:
  FREE( i );
  return error;
}
