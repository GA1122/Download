static HB_Error  Load_MarkLigPos( HB_GPOS_SubTable* st,
				  HB_Stream        stream )
{
  HB_Error  error;
  HB_MarkLigPos*  mlp = &st->marklig;

  HB_UInt  cur_offset, new_offset, base_offset;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 4L ) )
    return error;

  mlp->PosFormat = GET_UShort();
  new_offset     = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &mlp->MarkCoverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail3;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &mlp->LigatureCoverage,
				stream ) ) != HB_Err_Ok )
    goto Fail3;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 4L ) )
    goto Fail2;

  mlp->ClassCount = GET_UShort();
  new_offset      = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = Load_MarkArray( &mlp->MarkArray, stream ) ) != HB_Err_Ok )
    goto Fail2;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail1;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = Load_LigatureArray( &mlp->LigatureArray, mlp->ClassCount,
				     stream ) ) != HB_Err_Ok )
    goto Fail1;

  return HB_Err_Ok;

Fail1:
  Free_MarkArray( &mlp->MarkArray );

Fail2:
  _HB_OPEN_Free_Coverage( &mlp->LigatureCoverage );

Fail3:
  _HB_OPEN_Free_Coverage( &mlp->MarkCoverage );
  return error;
}
