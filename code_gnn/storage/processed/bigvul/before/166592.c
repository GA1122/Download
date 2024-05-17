bool WebGL2RenderingContextBase::ValidateAndUpdateBufferBindTarget(
    const char* function_name,
    GLenum target,
    WebGLBuffer* buffer) {
  if (!ValidateBufferTarget(function_name, target))
    return false;

  if (buffer &&
      !ValidateBufferTargetCompatibility(function_name, target, buffer))
    return false;

  switch (target) {
    case GL_ARRAY_BUFFER:
      bound_array_buffer_ = buffer;
      break;
    case GL_COPY_READ_BUFFER:
      bound_copy_read_buffer_ = buffer;
      break;
    case GL_COPY_WRITE_BUFFER:
      bound_copy_write_buffer_ = buffer;
      break;
    case GL_ELEMENT_ARRAY_BUFFER:
      bound_vertex_array_object_->SetElementArrayBuffer(buffer);
      break;
    case GL_PIXEL_PACK_BUFFER:
      bound_pixel_pack_buffer_ = buffer;
      break;
    case GL_PIXEL_UNPACK_BUFFER:
      bound_pixel_unpack_buffer_ = buffer;
      break;
    case GL_TRANSFORM_FEEDBACK_BUFFER:
      bound_transform_feedback_buffer_ = buffer;
      break;
    case GL_UNIFORM_BUFFER:
      bound_uniform_buffer_ = buffer;
      break;
    default:
      NOTREACHED();
      break;
  }

  if (buffer && !buffer->GetInitialTarget())
    buffer->SetInitialTarget(target);
  return true;
}
