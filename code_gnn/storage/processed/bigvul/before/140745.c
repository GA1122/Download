void GLES2DecoderImpl::DoMatrixLoadIdentityCHROMIUM(GLenum matrix_mode) {
  DCHECK(matrix_mode == GL_PATH_PROJECTION_CHROMIUM ||
         matrix_mode == GL_PATH_MODELVIEW_CHROMIUM);

  if (!features().chromium_path_rendering) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glMatrixLoadIdentityCHROMIUM",
                       "function not available");
    return;
  }

  GLfloat* target_matrix = matrix_mode == GL_PATH_PROJECTION_CHROMIUM
                               ? state_.projection_matrix
                               : state_.modelview_matrix;
  memcpy(target_matrix, kIdentityMatrix, sizeof(kIdentityMatrix));
  glMatrixLoadIdentityEXT(matrix_mode);
}
