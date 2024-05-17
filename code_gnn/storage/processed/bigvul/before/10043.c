  event_font_change( int  delta )
  {
    if ( status.font_index + delta >= handle->num_fonts ||
         status.font_index + delta < 0 )
      return;

    status.font_index += delta;

    FTDemo_Set_Current_Font( handle, handle->fonts[status.font_index] );
    FTDemo_Set_Current_Charsize( handle, status.ptsize, status.res );
    FTDemo_Update_Current_Flags( handle );

    FTDemo_String_Set( handle, Text );
  }