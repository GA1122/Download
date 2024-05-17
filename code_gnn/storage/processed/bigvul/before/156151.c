UnacceleratedStaticBitmapImage::UnacceleratedStaticBitmapImage(PaintImage image)
    : paint_image_(std::move(image)) {
   CHECK(paint_image_.GetSkImage());
 }
