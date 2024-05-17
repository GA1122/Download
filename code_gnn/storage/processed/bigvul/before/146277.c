PassRefPtr<Image> WebGLRenderingContextBase::DrawImageIntoBuffer(
    PassRefPtr<Image> pass_image,
    int width,
    int height,
    const char* function_name) {
  RefPtr<Image> image(std::move(pass_image));
  DCHECK(image);

  IntSize size(width, height);
  ImageBuffer* buf = generated_image_cache_.GetImageBuffer(size);
  if (!buf) {
    SynthesizeGLError(GL_OUT_OF_MEMORY, function_name, "out of memory");
    return nullptr;
  }

  if (!image->CurrentFrameKnownToBeOpaque())
    buf->Canvas()->clear(SK_ColorTRANSPARENT);

  IntRect src_rect(IntPoint(), image->Size());
  IntRect dest_rect(0, 0, size.Width(), size.Height());
  PaintFlags flags;
  image->Draw(buf->Canvas(), flags, dest_rect, src_rect,
              kDoNotRespectImageOrientation,
              Image::kDoNotClampImageToSourceRect);
  return buf->NewImageSnapshot(kPreferNoAcceleration,
                               kSnapshotReasonWebGLDrawImageIntoBuffer);
}
