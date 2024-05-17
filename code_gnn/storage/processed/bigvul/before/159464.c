void WebGLImageConversion::ImageExtractor::ExtractImage(
    bool premultiply_alpha,
    bool ignore_color_space) {
  DCHECK(!image_pixel_locker_);

  if (!image_)
    return;

  sk_sp<SkImage> skia_image = image_->PaintImageForCurrentFrame().GetSkImage();
  SkImageInfo info =
      skia_image ? SkImageInfo::MakeN32Premul(image_->width(), image_->height())
                 : SkImageInfo::MakeUnknown();
  alpha_op_ = kAlphaDoNothing;
  bool has_alpha = skia_image ? !skia_image->isOpaque() : true;

  if ((!skia_image || ignore_color_space ||
       (has_alpha && !premultiply_alpha)) &&
      image_->Data()) {
    std::unique_ptr<ImageDecoder> decoder(ImageDecoder::Create(
        image_->Data(), true, ImageDecoder::kAlphaNotPremultiplied,
        ignore_color_space ? ColorBehavior::Ignore()
                           : ColorBehavior::TransformToSRGB()));
    if (!decoder || !decoder->FrameCount())
      return;
    ImageFrame* frame = decoder->DecodeFrameBufferAtIndex(0);
    if (!frame || frame->GetStatus() != ImageFrame::kFrameComplete)
      return;
    has_alpha = frame->HasAlpha();
    SkBitmap bitmap = frame->Bitmap();
    if (!FrameIsValid(bitmap))
      return;

    skia_image = frame->FinalizePixelsAndGetImage();
    info = bitmap.info();

    if (has_alpha && premultiply_alpha)
      alpha_op_ = kAlphaDoPremultiply;
  } else if (!premultiply_alpha && has_alpha) {
    if (image_html_dom_source_ != kHtmlDomVideo)
      alpha_op_ = kAlphaDoUnmultiply;
  }

  if (!skia_image)
    return;

  image_source_format_ = SK_B32_SHIFT ? kDataFormatRGBA8 : kDataFormatBGRA8;
  image_source_unpack_alignment_ =
      0;   

  DCHECK(skia_image->width());
  DCHECK(skia_image->height());
  image_width_ = skia_image->width();
  image_height_ = skia_image->height();

  if (image_width_ != (unsigned)image_->width() ||
      image_height_ != (unsigned)image_->height())
    return;

  image_pixel_locker_.emplace(std::move(skia_image), info.alphaType(),
                              kN32_SkColorType);
}
