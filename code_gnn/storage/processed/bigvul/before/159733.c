void WebGLRenderingContextBase::TexImageCanvasByGPU(
    TexImageFunctionID function_id,
    HTMLCanvasElement* canvas,
    GLenum target,
    GLuint target_texture,
    GLint xoffset,
    GLint yoffset,
    const IntRect& source_sub_rectangle) {
  if (!canvas->Is3d()) {
    if (Extensions3DUtil::CanUseCopyTextureCHROMIUM(target) &&
        canvas->TryCreateImageBuffer()) {
      scoped_refptr<StaticBitmapImage> image =
          canvas->Canvas2DBuffer()->NewImageSnapshot(kPreferAcceleration);
      if (!!image && image->CopyToTexture(
                         ContextGL(), target, target_texture,
                         unpack_premultiply_alpha_, unpack_flip_y_,
                         IntPoint(xoffset, yoffset), source_sub_rectangle)) {
        return;
      }
    }
    NOTREACHED();
  } else {
    WebGLRenderingContextBase* gl =
        ToWebGLRenderingContextBase(canvas->RenderingContext());
    ScopedTexture2DRestorer restorer(gl);
    if (!gl->GetDrawingBuffer()->CopyToPlatformTexture(
            ContextGL(), target, target_texture, unpack_premultiply_alpha_,
            !unpack_flip_y_, IntPoint(xoffset, yoffset), source_sub_rectangle,
            kBackBuffer)) {
      NOTREACHED();
    }
  }
}
