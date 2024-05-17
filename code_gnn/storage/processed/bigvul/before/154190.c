  bool GetTransforms(const Cmd& cmd,
                     GLuint num_paths,
                     GLenum transform_type,
                     const GLfloat** out_transforms) {
    if (transform_type == GL_NONE) {
      *out_transforms = nullptr;
      return true;
    }
    uint32_t transforms_shm_id =
        static_cast<uint32_t>(cmd.transformValues_shm_id);
    uint32_t transforms_shm_offset =
        static_cast<uint32_t>(cmd.transformValues_shm_offset);
    uint32_t transforms_component_count =
        GLES2Util::GetComponentCountForGLTransformType(transform_type);
    DCHECK_LE(transforms_component_count, 12U);
    uint32_t one_transform_size = sizeof(GLfloat) * transforms_component_count;
    uint32_t transforms_size = 0;
    if (!base::CheckMul(one_transform_size, num_paths)
             .AssignIfValid(&transforms_size)) {
      error_ = error::kOutOfBounds;
      return false;
    }
    const GLfloat* transforms = nullptr;
    if (transforms_shm_id != 0 || transforms_shm_offset != 0)
      transforms = decoder_->GetSharedMemoryAs<const GLfloat*>(
          transforms_shm_id, transforms_shm_offset, transforms_size);
    if (!transforms) {
      error_ = error::kOutOfBounds;
      return false;
    }
    *out_transforms = transforms;
    return true;
  }
