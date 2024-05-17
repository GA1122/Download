 bool PPB_ImageData_Impl::Init(PP_ImageDataFormat format,
                               int width, int height,
                               bool init_to_zero) {
   if (!IsImageDataFormatSupported(format))
      return false;   
    if (width <= 0 || height <= 0)
      return false;
  if (static_cast<int64>(width) * static_cast<int64>(height) * 4 >=
      std::numeric_limits<int32>::max())
//   if (static_cast<int64>(width) * static_cast<int64>(height) >=
//       std::numeric_limits<int32>::max() / 4)
      return false;   
  
    format_ = format;
   width_ = width;
   height_ = height;
   return backend_->Init(this, format, width, height, init_to_zero);
 }