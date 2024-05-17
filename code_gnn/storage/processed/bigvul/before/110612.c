bool GLES2DecoderImpl::IsDrawValid(
    const char* function_name, GLuint max_vertex_accessed, GLsizei primcount) {
  if (!current_program_) {
    RenderWarning("Drawing with no current shader program.");
    return false;
  }

  bool divisor0 = false;
  const VertexAttribManager::VertexAttribInfoList& infos =
      vertex_attrib_manager_->GetEnabledVertexAttribInfos();
  for (VertexAttribManager::VertexAttribInfoList::const_iterator it =
       infos.begin(); it != infos.end(); ++it) {
    const VertexAttribManager::VertexAttribInfo* info = *it;
    const ProgramManager::ProgramInfo::VertexAttribInfo* attrib_info =
        current_program_->GetAttribInfoByLocation(info->index());
    if (attrib_info) {
      divisor0 |= (info->divisor() == 0);
      GLuint count = info->MaxVertexAccessed(primcount, max_vertex_accessed);
      if (!info->CanAccess(count)) {
        SetGLError(
            GL_INVALID_OPERATION, function_name,
            (std::string(
                 "attempt to access out of range vertices in attribute ") +
             base::IntToString(info->index())).c_str());
        return false;
      }
    } else {
      if (!info->buffer()) {
        SetGLError(
            GL_INVALID_OPERATION, function_name,
            (std::string(
                 "attempt to render with no buffer attached to "
                 "enabled attribute ") +
                 base::IntToString(info->index())).c_str());
        return false;
      }
    }
  }

  if (primcount && !divisor0) {
    SetGLError(
        GL_INVALID_OPERATION, function_name,
        "attempt instanced render with all attributes having "
        "non-zero divisors");
    return false;
  }

  return true;
}
