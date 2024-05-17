void GLES2DecoderImpl::DoMatrixLoadIdentityCHROMIUM(GLenum matrix_mode) {
  DCHECK(matrix_mode == GL_PATH_PROJECTION_CHROMIUM ||
         matrix_mode == GL_PATH_MODELVIEW_CHROMIUM);

  GLfloat* target_matrix = matrix_mode == GL_PATH_PROJECTION_CHROMIUM
                               ? state_.projection_matrix
                               : state_.modelview_matrix;
  memcpy(target_matrix, kIdentityMatrix, sizeof(kIdentityMatrix));
  api()->glMatrixLoadIdentityEXTFn(matrix_mode);
}
