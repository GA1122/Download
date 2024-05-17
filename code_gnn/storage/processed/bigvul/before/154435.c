void GLES2DecoderPassthroughImpl::EmulatedDefaultFramebuffer::Destroy(
    bool have_context) {
  if (have_context) {
    api->glDeleteFramebuffersEXTFn(1, &framebuffer_service_id);
    framebuffer_service_id = 0;

    api->glDeleteRenderbuffersEXTFn(1, &color_buffer_service_id);
    color_buffer_service_id = 0;

    api->glDeleteRenderbuffersEXTFn(1, &depth_stencil_buffer_service_id);
    color_buffer_service_id = 0;

    api->glDeleteRenderbuffersEXTFn(1, &depth_buffer_service_id);
    depth_buffer_service_id = 0;

    api->glDeleteRenderbuffersEXTFn(1, &stencil_buffer_service_id);
    stencil_buffer_service_id = 0;
  }
  if (color_texture) {
    color_texture->Destroy(have_context);
  }
}
