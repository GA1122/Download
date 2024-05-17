uint32_t GLES2Util::MapBufferTargetToBindingEnum(uint32_t target) {
  switch (target) {
    case GL_ARRAY_BUFFER:
      return GL_ARRAY_BUFFER_BINDING;
    case GL_ATOMIC_COUNTER_BUFFER:
      return GL_ATOMIC_COUNTER_BUFFER_BINDING;
    case GL_COPY_READ_BUFFER:
      return GL_COPY_READ_BUFFER_BINDING;
    case GL_COPY_WRITE_BUFFER:
      return GL_COPY_WRITE_BUFFER_BINDING;
    case GL_ELEMENT_ARRAY_BUFFER:
      return GL_ELEMENT_ARRAY_BUFFER_BINDING;
    case GL_PIXEL_PACK_BUFFER:
      return GL_PIXEL_PACK_BUFFER_BINDING;
    case GL_PIXEL_UNPACK_BUFFER:
      return GL_PIXEL_UNPACK_BUFFER_BINDING;
    case GL_SHADER_STORAGE_BUFFER:
      return GL_SHADER_STORAGE_BUFFER_BINDING;
    case GL_TRANSFORM_FEEDBACK_BUFFER:
      return GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
    case GL_UNIFORM_BUFFER:
      return GL_UNIFORM_BUFFER_BINDING;
    default:
      return 0;
  }
}