  gray_raster_done( FT_Raster  raster )
  {
    FT_Memory  memory = (FT_Memory)((PRaster)raster)->memory;


    FT_FREE( raster );
  }
