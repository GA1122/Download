  bool GetCoverMode(const Cmd& cmd, GLenum* out_cover_mode) {
    GLenum cover_mode = static_cast<GLuint>(cmd.coverMode);
    if (!validators_->path_instanced_cover_mode.IsValid(cover_mode)) {
      ERRORSTATE_SET_GL_ERROR_INVALID_ENUM(error_state_, function_name_,
                                           cover_mode, "coverMode");
      return false;
    }
    *out_cover_mode = cover_mode;
    return true;
  }
