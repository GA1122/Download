bool GLES2DecoderImpl::GenPathsCHROMIUMHelper(GLuint first_client_id,
                                              GLsizei range) {
  GLuint last_client_id;
  if (range < 1 || !base::CheckAdd(first_client_id, range - 1)
                        .AssignIfValid(&last_client_id))
    return false;

  if (path_manager()->HasPathsInRange(first_client_id, last_client_id))
    return false;

  GLuint first_service_id = api()->glGenPathsNVFn(range);
  if (first_service_id == 0) {
    return false;
  }
  DCHECK(first_service_id + range - 1 >= first_service_id);

  path_manager()->CreatePathRange(first_client_id, last_client_id,
                                  first_service_id);

  return true;
}
