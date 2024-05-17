error::Error GLES2DecoderPassthroughImpl::DoDispatchCompute(
    GLuint num_groups_x,
    GLuint num_groups_y,
    GLuint num_groups_z) {
  BindPendingImagesForSamplersIfNeeded();
  api()->glDispatchComputeFn(num_groups_x, num_groups_y, num_groups_z);
  return error::kNoError;
}
