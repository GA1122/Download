void GLES2DecoderImpl::GetVertexAttribHelper(
    const VertexAttrib* attrib, GLenum pname, GLint* params) {
  switch (pname) {
    case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
      {
        Buffer* buffer = attrib->buffer();
        if (buffer && !buffer->IsDeleted()) {
          GLuint client_id;
          buffer_manager()->GetClientId(buffer->service_id(), &client_id);
          *params = client_id;
        }
        break;
      }
    case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
      *params = attrib->enabled();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:
      *params = attrib->size();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
      *params = attrib->gl_stride();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:
      *params = attrib->type();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
      *params = attrib->normalized();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_DIVISOR:
      *params = attrib->divisor();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_INTEGER:
      *params = attrib->integer();
      break;
    default:
      NOTREACHED();
      break;
  }
}
