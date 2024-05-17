  bool GetPathCountAndType(const Cmd& cmd,
                           GLuint* out_num_paths,
                           GLenum* out_path_name_type) {
    int32_t numPaths = cmd.numPaths;
    if (numPaths < 0) {
      ERRORSTATE_SET_GL_ERROR(error_state_, GL_INVALID_VALUE, function_name_,
                              "numPaths < 0");
      return false;
    }
    GLenum path_name_type = static_cast<GLenum>(cmd.pathNameType);
    if (!validators_->path_name_type.IsValid(path_name_type)) {
      ERRORSTATE_SET_GL_ERROR_INVALID_ENUM(error_state_, function_name_,
                                           path_name_type, "pathNameType");
      return false;
    }
    *out_num_paths = static_cast<GLsizei>(numPaths);
    *out_path_name_type = path_name_type;
    return true;
  }
