Extensions3DUtil* WebGLRenderingContextBase::ExtensionsUtil() {
  if (!extensions_util_) {
    gpu::gles2::GLES2Interface* gl = ContextGL();
    extensions_util_ = Extensions3DUtil::Create(gl);
    DCHECK(extensions_util_->IsValid() ||
           gl->GetGraphicsResetStatusKHR() != GL_NO_ERROR);
  }
  return extensions_util_.get();
}
