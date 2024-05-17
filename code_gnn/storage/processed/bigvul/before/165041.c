scoped_refptr<StaticBitmapImage> HTMLCanvasElement::Snapshot(
    SourceDrawingBuffer source_buffer,
    AccelerationHint hint) const {
  if (size_.IsEmpty())
    return nullptr;

  scoped_refptr<StaticBitmapImage> image_bitmap = nullptr;
  if (PlaceholderFrame()) {   
    DCHECK(PlaceholderFrame()->OriginClean());
    image_bitmap = PlaceholderFrame()->Bitmap();
  } else if (Is3d()) {   
    if (context_->CreationAttributes().premultiplied_alpha) {
      context_->PaintRenderingResultsToCanvas(source_buffer);
      if (ResourceProvider())
        image_bitmap = ResourceProvider()->Snapshot();
    } else {
      scoped_refptr<Uint8Array> data_array =
          context_->PaintRenderingResultsToDataArray(source_buffer);
      if (data_array) {
        IntSize adjusted_size = context_->DrawingBufferSize();
        SkImageInfo info =
            SkImageInfo::Make(adjusted_size.Width(), adjusted_size.Height(),
                              kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
        info = info.makeColorSpace(ColorParams().GetSkColorSpace());
        if (ColorParams().GetSkColorType() != kN32_SkColorType)
          info = info.makeColorType(kRGBA_F16_SkColorType);
        image_bitmap = StaticBitmapImage::Create(std::move(data_array), info);
      }
    }
  } else if (canvas2d_bridge_) {   
    DCHECK(Is2d());
    image_bitmap = canvas2d_bridge_->NewImageSnapshot(hint);
  } else if (context_) {   
    image_bitmap = context_->GetImage(hint);
  }

  if (!image_bitmap)
    image_bitmap = CreateTransparentImage(size_);
  return image_bitmap;
}
