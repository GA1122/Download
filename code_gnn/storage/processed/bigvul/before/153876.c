bool GLES2DecoderImpl::AttribsTypeMatch() {
  if (!state_.current_program.get())
    return true;
  const std::vector<uint32_t>& shader_attrib_active_mask =
      state_.current_program->vertex_input_active_mask();
  const std::vector<uint32_t>& shader_attrib_type_mask =
      state_.current_program->vertex_input_base_type_mask();
  const std::vector<uint32_t>& generic_vertex_attrib_type_mask =
      state_.generic_attrib_base_type_mask();
  const std::vector<uint32_t>& vertex_attrib_array_enabled_mask =
      state_.vertex_attrib_manager->attrib_enabled_mask();
  const std::vector<uint32_t>& vertex_attrib_array_type_mask =
      state_.vertex_attrib_manager->attrib_base_type_mask();
  DCHECK_EQ(shader_attrib_active_mask.size(),
            shader_attrib_type_mask.size());
  DCHECK_EQ(shader_attrib_active_mask.size(),
            generic_vertex_attrib_type_mask.size());
  DCHECK_EQ(shader_attrib_active_mask.size(),
            vertex_attrib_array_enabled_mask.size());
  DCHECK_EQ(shader_attrib_active_mask.size(),
            vertex_attrib_array_type_mask.size());
  for (size_t ii = 0; ii < shader_attrib_active_mask.size(); ++ii) {
    uint32_t vertex_attrib_source_type_mask =
        (~vertex_attrib_array_enabled_mask[ii] &
         generic_vertex_attrib_type_mask[ii]) |
        (vertex_attrib_array_enabled_mask[ii] &
         vertex_attrib_array_type_mask[ii]);
    if ((shader_attrib_type_mask[ii] & shader_attrib_active_mask[ii]) !=
        (vertex_attrib_source_type_mask & shader_attrib_active_mask[ii])) {
      return false;
    }
  }
  return true;
}
