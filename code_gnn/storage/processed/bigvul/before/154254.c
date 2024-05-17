error::Error GLES2DecoderImpl::HandleInitializeDiscardableTextureCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::InitializeDiscardableTextureCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::InitializeDiscardableTextureCHROMIUM*>(
          cmd_data);
  GLuint texture_id = c.texture_id;
  uint32_t shm_id = c.shm_id;
  uint32_t shm_offset = c.shm_offset;

  TextureRef* texture = texture_manager()->GetTexture(texture_id);
  if (!texture) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE,
                       "glInitializeDiscardableTextureCHROMIUM",
                       "Invalid texture ID");
    return error::kNoError;
  }
  scoped_refptr<gpu::Buffer> buffer = GetSharedMemoryBuffer(shm_id);
  if (!DiscardableHandleBase::ValidateParameters(buffer.get(), shm_offset))
    return error::kInvalidArguments;

  size_t size = texture->texture()->estimated_size();
  ServiceDiscardableHandle handle(std::move(buffer), shm_offset, shm_id);
  GetContextGroup()->discardable_manager()->InsertLockedTexture(
      texture_id, size, group_->texture_manager(), std::move(handle));
  return error::kNoError;
}
