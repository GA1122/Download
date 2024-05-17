bool GLES2DecoderImpl::DoIsVertexArrayOES(GLuint client_id) {
  const VertexAttribManager* vao =
      GetVertexAttribManager(client_id);
  return vao && vao->IsValid() && !vao->IsDeleted();
}
