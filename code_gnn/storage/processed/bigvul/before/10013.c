  event_grid_zoom( double  zoom )
  {
    status.scale *= zoom;

    sprintf( status.header_buffer, "zoom level %.2f %%\n",
             status.scale / status.scale_0 );

    status.header = (const char *)status.header_buffer;
  }
