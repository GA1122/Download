  bool GetFillMode(const Cmd& cmd, GLenum* out_fill_mode) {
    GLenum fill_mode = static_cast<GLenum>(cmd.fillMode);
    if (!validators_->path_fill_mode.IsValid(fill_mode)) {
      ERRORSTATE_SET_GL_ERROR_INVALID_ENUM(error_state_, function_name_,
                                           fill_mode, "fillMode");
      return false;
    }
    *out_fill_mode = fill_mode;
    return true;
  }
