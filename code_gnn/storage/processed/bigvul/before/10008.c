circle_draw( FT_F26Dot6       center_x,
             FT_F26Dot6       center_y,
             FT_F26Dot6       radius,
             FTDemo_Handle*   handle,
             FTDemo_Display*  display,
             grColor          color )
{
  FT_Outline  outline;

  ft_outline_new_circle( &outline, radius, handle );
  FT_Outline_Translate( &outline, center_x & 63, center_y & 63 );

  ft_outline_draw( &outline, 1., (center_x >> 6), (center_y >> 6), handle, display, color );

  FT_Outline_Done( handle->library, &outline );
}
