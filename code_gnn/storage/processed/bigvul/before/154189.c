  bool GetTransformType(const Cmd& cmd, GLenum* out_transform_type) {
    GLenum transform_type = static_cast<GLenum>(cmd.transformType);
    if (!validators_->path_transform_type.IsValid(transform_type)) {
      ERRORSTATE_SET_GL_ERROR_INVALID_ENUM(error_state_, function_name_,
                                           transform_type, "transformType");
      return false;
    }
    *out_transform_type = transform_type;
    return true;
  }
