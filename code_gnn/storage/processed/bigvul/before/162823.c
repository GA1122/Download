void ContextState::SetBoundBuffer(GLenum target, Buffer* buffer) {
  switch (target) {
    case GL_ARRAY_BUFFER:
      bound_array_buffer = buffer;
      break;
    case GL_ELEMENT_ARRAY_BUFFER:
      vertex_attrib_manager->SetElementArrayBuffer(buffer);
      break;
    case GL_COPY_READ_BUFFER:
      bound_copy_read_buffer = buffer;
      break;
    case GL_COPY_WRITE_BUFFER:
      bound_copy_write_buffer = buffer;
      break;
    case GL_PIXEL_PACK_BUFFER:
      bound_pixel_pack_buffer = buffer;
      UpdatePackParameters();
      break;
    case GL_PIXEL_UNPACK_BUFFER:
      bound_pixel_unpack_buffer = buffer;
      UpdateUnpackParameters();
      break;
    case GL_TRANSFORM_FEEDBACK_BUFFER:
      bound_transform_feedback_buffer = buffer;
      break;
    case GL_UNIFORM_BUFFER:
      bound_uniform_buffer = buffer;
      break;
    default:
      NOTREACHED();
      break;
  }
}
