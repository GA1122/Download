void ContextState::RestoreVertexAttribs() const {
  DCHECK(vertex_attrib_manager.get());
  if (feature_info_->feature_flags().native_vertex_array_object) {
    GLuint default_vao_service_id =
        default_vertex_attrib_manager->service_id();
    if (default_vao_service_id == 0)
      RestoreVertexAttribArrays(default_vertex_attrib_manager);

    GLuint curr_vao_service_id = vertex_attrib_manager->service_id();
    if (curr_vao_service_id != 0)
      api()->glBindVertexArrayOESFn(curr_vao_service_id);
  } else {
    RestoreVertexAttribArrays(vertex_attrib_manager);
  }

  RestoreVertexAttribValues();
}
