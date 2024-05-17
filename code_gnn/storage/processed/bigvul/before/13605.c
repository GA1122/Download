static HB_Error  Lookup_CursivePos( GPOS_Instance*    gpi,
				    HB_GPOS_SubTable* st,
				    HB_Buffer        buffer,
				    HB_UShort         flags,
				    HB_UShort         context_length,
				    int               nesting_level )
{
  HB_UShort        index, property;
  HB_Error         error;
  HB_GPOSHeader*  gpos = gpi->gpos;
  HB_CursivePos*  cp = &st->cursive;

  HB_EntryExitRecord*  eer;
  HB_Fixed                entry_x, entry_y;
  HB_Fixed                exit_x, exit_y;

  HB_UNUSED(nesting_level);

  if ( context_length != 0xFFFF && context_length < 1 )
  {
    gpi->last = 0xFFFF;
    return HB_Err_Not_Covered;
  }

   

  if ( CHECK_Property( gpos->gdef, IN_CURITEM(), flags, &property ) )
    return error;

   

  if ( property == HB_GDEF_MARK )
  {
    gpi->last = 0xFFFF;
    return HB_Err_Not_Covered;
  }

  error = _HB_OPEN_Coverage_Index( &cp->Coverage, IN_CURGLYPH(), &index );
  if ( error )
  {
    gpi->last = 0xFFFF;
    return error;
  }

  if ( index >= cp->EntryExitCount )
    return ERR(HB_Err_Invalid_SubTable);

  eer = &cp->EntryExitRecord[index];

   

  if ( gpi->last == 0xFFFF )
    goto end;

   

  error = Get_Anchor( gpi, &eer->EntryAnchor, IN_CURGLYPH(),
		      &entry_x, &entry_y );
  if ( error == HB_Err_Not_Covered )
    goto end;
  if ( error )
    return error;

  if ( gpi->r2l )
  {
    POSITION( buffer->in_pos )->x_advance   = entry_x - gpi->anchor_x;
    POSITION( buffer->in_pos )->new_advance = TRUE;
  }
  else
  {
    POSITION( gpi->last )->x_advance   = gpi->anchor_x - entry_x;
    POSITION( gpi->last )->new_advance = TRUE;
  }

  if ( flags & HB_LOOKUP_FLAG_RIGHT_TO_LEFT )
  {
    POSITION( gpi->last )->cursive_chain = gpi->last - buffer->in_pos;
    POSITION( gpi->last )->y_pos = entry_y - gpi->anchor_y;
  }
  else
  {
    POSITION( buffer->in_pos )->cursive_chain = buffer->in_pos - gpi->last;
    POSITION( buffer->in_pos )->y_pos = gpi->anchor_y - entry_y;
  }

end:
  error = Get_Anchor( gpi, &eer->ExitAnchor, IN_CURGLYPH(),
		      &exit_x, &exit_y );
  if ( error == HB_Err_Not_Covered )
    gpi->last = 0xFFFF;
  else
  {
    gpi->last     = buffer->in_pos;
    gpi->anchor_x = exit_x;
    gpi->anchor_y = exit_y;
  }
  if ( error )
    return error;

  (buffer->in_pos)++;

  return HB_Err_Ok;
}
