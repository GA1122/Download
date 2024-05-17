  bool GetFillModeAndMask(const Cmd& cmd,
                          GLenum* out_fill_mode,
                          GLuint* out_mask) {
    GLenum fill_mode;
    if (!GetFillMode(cmd, &fill_mode))
      return false;
    GLuint mask = static_cast<GLuint>(cmd.mask);
     
    if ((fill_mode == GL_COUNT_UP_CHROMIUM ||
         fill_mode == GL_COUNT_DOWN_CHROMIUM) &&
        GLES2Util::IsNPOT(mask + 1)) {
      ERRORSTATE_SET_GL_ERROR(error_state_, GL_INVALID_VALUE, function_name_,
                              "mask+1 is not power of two");
      return false;
    }
    *out_fill_mode = fill_mode;
    *out_mask = mask;
    return true;
  }
