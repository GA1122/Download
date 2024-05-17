static HB_Error  Position_CursiveChain ( HB_Buffer     buffer )
{
  HB_UInt   i, j;
  HB_Position positions = buffer->positions;

   
  for (j = 0; j < buffer->in_length; j++)
  {
    if (positions[j].cursive_chain > 0)
      positions[j].y_pos += positions[j - positions[j].cursive_chain].y_pos;
  }

   
  for (i = buffer->in_length; i > 0; i--)
  {
    j = i - 1;

    if (positions[j].cursive_chain < 0)
      positions[j].y_pos += positions[j - positions[j].cursive_chain].y_pos;
  }

  return HB_Err_Ok;
}