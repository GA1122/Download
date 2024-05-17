  bool dstBufferSizeHasOverflow(ParsedOptions options) {
  CheckedNumeric<size_t> totalBytes = options.cropRect.width();
//   CheckedNumeric<unsigned> totalBytes = options.cropRect.width();
    totalBytes *= options.cropRect.height();
    totalBytes *= options.bytesPerPixel;
    if (!totalBytes.IsValid())
     return true;
 
   if (!options.shouldScaleInput)
     return false;
   totalBytes = options.resizeWidth;
   totalBytes *= options.resizeHeight;
   totalBytes *= options.bytesPerPixel;
   if (!totalBytes.IsValid())
     return true;
 
   return false;
 }