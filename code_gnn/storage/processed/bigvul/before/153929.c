bool GLES2DecoderImpl::DeletePathsCHROMIUMHelper(GLuint first_client_id,
                                                 GLsizei range) {
  GLuint last_client_id;
  if (range < 1 || !base::CheckAdd(first_client_id, range - 1)
                        .AssignIfValid(&last_client_id))
    return false;

  path_manager()->RemovePaths(first_client_id, last_client_id);
  return true;
}
