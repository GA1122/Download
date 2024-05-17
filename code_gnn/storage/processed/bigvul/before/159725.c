scoped_refptr<Image> WebGLRenderingContextBase::DrawImageIntoBuffer(
    scoped_refptr<Image> pass_image,
    int width,
    int height,
    const char* function_name) {
  scoped_refptr<Image> image(std::move(pass_image));
  DCHECK(image);

  IntSize size(width, height);
  CanvasResourceProvider* resource_provider =
      generated_image_cache_.GetCanvasResourceProvider(size);
  if (!resource_provider) {
    SynthesizeGLError(GL_OUT_OF_MEMORY, function_name, "out of memory");
    return nullptr;
  }

  if (!image->CurrentFrameKnownToBeOpaque())
    resource_provider->Canvas()->clear(SK_ColorTRANSPARENT);

  IntRect src_rect(IntPoint(), image->Size());
  IntRect dest_rect(0, 0, size.Width(), size.Height());
  PaintFlags flags;
  image->Draw(resource_provider->Canvas(), flags, dest_rect, src_rect,
              kDoNotRespectImageOrientation,
              Image::kDoNotClampImageToSourceRect, Image::kSyncDecode);
  return resource_provider->Snapshot();
}
