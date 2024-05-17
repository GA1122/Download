WebGL2RenderingContextBase::WebGL2RenderingContextBase(
    CanvasRenderingContextHost* host,
    std::unique_ptr<WebGraphicsContext3DProvider> context_provider,
    const CanvasContextCreationAttributes& requested_attributes)
    : WebGLRenderingContextBase(host,
                                std::move(context_provider),
                                requested_attributes,
                                2),
      read_framebuffer_binding_(this, nullptr),
      transform_feedback_binding_(this, nullptr),
      bound_copy_read_buffer_(this, nullptr),
      bound_copy_write_buffer_(this, nullptr),
      bound_pixel_pack_buffer_(this, nullptr),
      bound_pixel_unpack_buffer_(this, nullptr),
      bound_uniform_buffer_(this, nullptr),
      current_boolean_occlusion_query_(this, nullptr),
      current_transform_feedback_primitives_written_query_(this, nullptr),
      current_elapsed_query_(this, nullptr) {
  supported_internal_formats_storage_.insert(
      kSupportedInternalFormatsStorage,
      kSupportedInternalFormatsStorage +
          WTF_ARRAY_LENGTH(kSupportedInternalFormatsStorage));
}
