sk_sp<SkImage> WebGLRenderingContextBase::MakeImageSnapshot(
    SkImageInfo& image_info) {
  GetDrawingBuffer()->ResolveAndBindForReadAndDraw();
  gpu::gles2::GLES2Interface* gl = SharedGpuContext::Gl();

  SkSurfaceProps disable_lcd_props(0, kUnknown_SkPixelGeometry);
  sk_sp<SkSurface> surface = SkSurface::MakeRenderTarget(
      SharedGpuContext::Gr(), SkBudgeted::kYes, image_info, 0,
      image_info.alphaType() == kOpaque_SkAlphaType ? nullptr
                                                    : &disable_lcd_props);
  const GrGLTextureInfo* texture_info = skia::GrBackendObjectToGrGLTextureInfo(
      surface->getTextureHandle(SkSurface::kDiscardWrite_TextureHandleAccess));
  GLuint texture_id = texture_info->fID;
  GLenum texture_target = texture_info->fTarget;

  GetDrawingBuffer()->CopyToPlatformTexture(
      gl, texture_target, texture_id, true, false, IntPoint(0, 0),
      IntRect(IntPoint(0, 0), GetDrawingBuffer()->Size()), kBackBuffer);
  return surface->makeImageSnapshot();
}
