  bool GetPathRange(const Cmd& cmd, GLsizei* out_range) {
    GLsizei range = static_cast<GLsizei>(cmd.range);
    if (range < 0) {
      ERRORSTATE_SET_GL_ERROR(error_state_, GL_INVALID_VALUE, function_name_,
                              "range < 0");
      return false;
    }
    *out_range = range;
    return true;
  }
