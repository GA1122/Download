bool GLES2DecoderImpl::DoIsPathCHROMIUM(GLuint client_id) {
  GLuint service_id = 0;
  return path_manager()->GetPath(client_id, &service_id) &&
         api()->glIsPathNVFn(service_id) == GL_TRUE;
}
