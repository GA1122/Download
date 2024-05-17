void GLES2DecoderImpl::SetSurface(
    const scoped_refptr<gfx::GLSurface>& surface) {
  DCHECK(context_->IsCurrent(NULL));
  DCHECK(surface_.get());
  surface_ = surface;
  RestoreCurrentFramebufferBindings();
}
