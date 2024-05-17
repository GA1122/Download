void WebGLRenderingContextBase::RestoreCurrentTexture2D() {
  bindTexture(GL_TEXTURE_2D,
              texture_units_[active_texture_unit_].texture2d_binding_.Get());
}
