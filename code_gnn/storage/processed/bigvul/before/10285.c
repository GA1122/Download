  ft_stub_set_pixel_sizes( FT_Size  size,
                           FT_UInt  width,
                           FT_UInt  height )
  {
    FT_Size_RequestRec  req;
    FT_Driver           driver = size->face->driver;


    if ( driver->clazz->request_size )
    {
      req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
      req.width          = width  << 6;
      req.height         = height << 6;
      req.horiResolution = 0;
      req.vertResolution = 0;

      return driver->clazz->request_size( size, &req );
    }

    return 0;
  }
