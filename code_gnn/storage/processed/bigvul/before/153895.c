ALWAYS_INLINE bool GLES2DecoderImpl::CheckMultiDrawArraysVertices(
    const char* function_name,
    bool instanced,
    const GLint* firsts,
    const GLsizei* counts,
    const GLsizei* primcounts,
    GLsizei drawcount,
    GLuint* total_max_vertex_accessed,
    GLsizei* total_max_primcount) {
  DCHECK_GE(drawcount, 0);
  for (GLsizei draw_id = 0; draw_id < drawcount; ++draw_id) {
    GLint first = firsts[draw_id];
    GLsizei count = counts[draw_id];
    GLsizei primcount = instanced ? primcounts[draw_id] : 1;
    if (first < 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "first < 0");
      return false;
    }
    if (count < 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "count < 0");
      return false;
    }
    if (primcount < 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "primcount < 0");
      return false;
    }
    if (count == 0 || primcount == 0) {
      LOCAL_RENDER_WARNING("Render count or primcount is 0.");
      continue;
    }

    base::CheckedNumeric<GLuint> checked_max_vertex = first;
    checked_max_vertex += count - 1;
    GLuint max_vertex_accessed = 0;
    if (!checked_max_vertex.AssignIfValid(&max_vertex_accessed)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                         "first + count overflow");
      return false;
    }
    if (!IsDrawValid(function_name, max_vertex_accessed, instanced,
                     primcount)) {
      return false;
    }
    *total_max_vertex_accessed =
        std::max(*total_max_vertex_accessed, max_vertex_accessed);
    *total_max_primcount = std::max(*total_max_primcount, primcount);
  }
  return true;
}
