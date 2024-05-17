void GLES2DecoderImpl::ClearAllAttributes() const {
  if (feature_info_->feature_flags().native_vertex_array_object)
    api()->glBindVertexArrayOESFn(0);

  for (uint32_t i = 0; i < group_->max_vertex_attribs(); ++i) {
    if (i != 0)   
      state_.vertex_attrib_manager->SetDriverVertexAttribEnabled(i, false);
    if (features().angle_instanced_arrays)
      api()->glVertexAttribDivisorANGLEFn(i, 0);
  }
}
