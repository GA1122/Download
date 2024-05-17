WebGLBuffer* WebGL2RenderingContextBase::ValidateBufferDataTarget(
    const char* function_name,
    GLenum target) {
  WebGLBuffer* buffer = nullptr;
  switch (target) {
    case GL_ELEMENT_ARRAY_BUFFER:
      buffer = bound_vertex_array_object_->BoundElementArrayBuffer();
      break;
    case GL_ARRAY_BUFFER:
      buffer = bound_array_buffer_.Get();
      break;
    case GL_COPY_READ_BUFFER:
      buffer = bound_copy_read_buffer_.Get();
      break;
    case GL_COPY_WRITE_BUFFER:
      buffer = bound_copy_write_buffer_.Get();
      break;
    case GL_PIXEL_PACK_BUFFER:
      buffer = bound_pixel_pack_buffer_.Get();
      break;
    case GL_PIXEL_UNPACK_BUFFER:
      buffer = bound_pixel_unpack_buffer_.Get();
      break;
    case GL_TRANSFORM_FEEDBACK_BUFFER:
      buffer = transform_feedback_binding_->GetBoundTransformFeedbackBuffer();
      break;
    case GL_UNIFORM_BUFFER:
      buffer = bound_uniform_buffer_.Get();
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid target");
      return nullptr;
  }
  if (!buffer) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name, "no buffer");
    return nullptr;
  }
  return buffer;
}
