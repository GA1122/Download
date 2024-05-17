void GLES2DecoderImpl::SetSurface(const scoped_refptr<gl::GLSurface>& surface) {
  DCHECK(context_->IsCurrent(nullptr));
  DCHECK(surface);
  surface_ = surface;
  RestoreCurrentFramebufferBindings();
}
