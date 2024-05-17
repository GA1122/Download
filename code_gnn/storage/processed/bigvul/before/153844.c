void GLES2Implementation::UnmapTexSubImage2DCHROMIUM(const void* mem) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glUnmapTexSubImage2DCHROMIUM("
                     << mem << ")");
  MappedTextureMap::iterator it = mapped_textures_.find(mem);
  if (it == mapped_textures_.end()) {
    SetGLError(GL_INVALID_VALUE, "UnmapTexSubImage2DCHROMIUM",
               "texture not mapped");
    return;
  }
  const MappedTexture& mt = it->second;
  helper_->TexSubImage2D(mt.target, mt.level, mt.xoffset, mt.yoffset, mt.width,
                         mt.height, mt.format, mt.type, mt.shm_id,
                         mt.shm_offset, GL_FALSE);
  mapped_memory_->FreePendingToken(mt.shm_memory, helper_->InsertToken());
  mapped_textures_.erase(it);
  CheckGLError();
}
