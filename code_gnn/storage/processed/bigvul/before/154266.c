error::Error GLES2DecoderImpl::HandleProgramPathFragmentInputGenCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glProgramPathFragmentInputGenCHROMIUM";
  const volatile gles2::cmds::ProgramPathFragmentInputGenCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::ProgramPathFragmentInputGenCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering) {
    return error::kUnknownCommand;
  }

  GLint program_id = static_cast<GLint>(c.program);

  Program* program = GetProgram(program_id);
  if (!program || !program->IsValid() || program->IsDeleted()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName, "invalid program");
    return error::kNoError;
  }

  GLenum gen_mode = static_cast<GLint>(c.genMode);
  if (!validators_->path_fragment_input_gen_mode.IsValid(gen_mode)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(kFunctionName, gen_mode, "genMode");
    return error::kNoError;
  }

  GLint components = static_cast<GLint>(c.components);
  if (components < 0 || components > 4) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                       "components out of range");
    return error::kNoError;
  }

  if ((components != 0 && gen_mode == GL_NONE) ||
      (components == 0 && gen_mode != GL_NONE)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                       "components and genMode do not match");
    return error::kNoError;
  }

  GLint location = static_cast<GLint>(c.location);
  if (program->IsInactiveFragmentInputLocationByFakeLocation(location))
    return error::kNoError;

  const Program::FragmentInputInfo* fragment_input_info =
      program->GetFragmentInputInfoByFakeLocation(location);
  if (!fragment_input_info) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName, "unknown location");
    return error::kNoError;
  }
  GLint real_location = fragment_input_info->location;

  const GLfloat* coeffs = nullptr;

  if (components > 0) {
    GLint components_needed = -1;

    switch (fragment_input_info->type) {
      case GL_FLOAT:
        components_needed = 1;
        break;
      case GL_FLOAT_VEC2:
        components_needed = 2;
        break;
      case GL_FLOAT_VEC3:
        components_needed = 3;
        break;
      case GL_FLOAT_VEC4:
        components_needed = 4;
        break;
      default:
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                           "fragment input type is not single-precision "
                           "floating-point scalar or vector");
        return error::kNoError;
    }

    if (components_needed != components) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                         "components does not match fragment input type");
      return error::kNoError;
    }
    uint32_t coeffs_per_component =
        GLES2Util::GetCoefficientCountForGLPathFragmentInputGenMode(gen_mode);
    DCHECK(coeffs_per_component > 0 && coeffs_per_component <= 4);
    DCHECK(components > 0 && components <= 4);
    uint32_t coeffs_size = sizeof(GLfloat) * coeffs_per_component * components;

    uint32_t coeffs_shm_id = static_cast<uint32_t>(c.coeffs_shm_id);
    uint32_t coeffs_shm_offset = static_cast<uint32_t>(c.coeffs_shm_offset);

    if (coeffs_shm_id != 0 || coeffs_shm_offset != 0) {
      coeffs = GetSharedMemoryAs<const GLfloat*>(
          coeffs_shm_id, coeffs_shm_offset, coeffs_size);
    }

    if (!coeffs) {
      return error::kOutOfBounds;
    }
  }
  api()->glProgramPathFragmentInputGenNVFn(program->service_id(), real_location,
                                           gen_mode, components, coeffs);
  return error::kNoError;
}
