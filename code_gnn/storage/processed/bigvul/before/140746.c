void GLES2DecoderImpl::DoMatrixLoadfCHROMIUM(GLenum matrix_mode,
                                             const GLfloat* matrix) {
  DCHECK(matrix_mode == GL_PATH_PROJECTION_CHROMIUM ||
         matrix_mode == GL_PATH_MODELVIEW_CHROMIUM);
  if (!features().chromium_path_rendering) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glMatrixLoadfCHROMIUM",
                       "function not available");
    return;
  }

  GLfloat* target_matrix = matrix_mode == GL_PATH_PROJECTION_CHROMIUM
                               ? state_.projection_matrix
                               : state_.modelview_matrix;
  memcpy(target_matrix, matrix, sizeof(GLfloat) * 16);
  glMatrixLoadfEXT(matrix_mode, matrix);
}
