bool GLES2DecoderImpl::DeletePathsCHROMIUMHelper(GLuint first_client_id,
                                                 GLsizei range) {
  GLuint last_client_id;
  if (!SafeAddUint32(first_client_id, range - 1, &last_client_id))
    return false;

  path_manager()->RemovePaths(first_client_id, last_client_id);
  return true;
}
