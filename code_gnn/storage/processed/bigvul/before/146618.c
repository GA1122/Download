bool DrawingBuffer::CopyToPlatformTexture(gpu::gles2::GLES2Interface* gl,
                                          GLenum texture_target,
                                          GLuint texture,
                                          bool premultiply_alpha,
                                          bool flip_y,
                                          const IntPoint& dest_texture_offset,
                                          const IntRect& source_sub_rectangle,
                                          SourceDrawingBuffer source_buffer) {
  ScopedStateRestorer scoped_state_restorer(this);

  if (contents_changed_) {
    ResolveIfNeeded();
    gl_->Flush();
  }

  if (!Extensions3DUtil::CanUseCopyTextureCHROMIUM(texture_target))
    return false;

  GLenum target = 0;
  gpu::Mailbox mailbox;
  gpu::SyncToken produce_sync_token;
  if (source_buffer == kFrontBuffer && front_color_buffer_) {
    target = front_color_buffer_->parameters.target;
    mailbox = front_color_buffer_->mailbox;
    produce_sync_token = front_color_buffer_->produce_sync_token;
  } else {
    target = back_color_buffer_->parameters.target;
    gl_->GenMailboxCHROMIUM(mailbox.name);
    gl_->ProduceTextureDirectCHROMIUM(back_color_buffer_->texture_id, target,
                                      mailbox.name);
    const GLuint64 fence_sync = gl_->InsertFenceSyncCHROMIUM();
    gl_->Flush();
    gl_->GenSyncTokenCHROMIUM(fence_sync, produce_sync_token.GetData());
  }

  if (!produce_sync_token.HasData()) {
    return false;
  }

  gl->WaitSyncTokenCHROMIUM(produce_sync_token.GetConstData());
  GLuint source_texture =
      gl->CreateAndConsumeTextureCHROMIUM(target, mailbox.name);

  GLboolean unpack_premultiply_alpha_needed = GL_FALSE;
  GLboolean unpack_unpremultiply_alpha_needed = GL_FALSE;
  if (want_alpha_channel_ && premultiplied_alpha_ && !premultiply_alpha)
    unpack_unpremultiply_alpha_needed = GL_TRUE;
  else if (want_alpha_channel_ && !premultiplied_alpha_ && premultiply_alpha)
    unpack_premultiply_alpha_needed = GL_TRUE;

  gl->CopySubTextureCHROMIUM(
      source_texture, 0, texture_target, texture, 0, dest_texture_offset.X(),
      dest_texture_offset.Y(), source_sub_rectangle.X(),
      source_sub_rectangle.Y(), source_sub_rectangle.Width(),
      source_sub_rectangle.Height(), flip_y, unpack_premultiply_alpha_needed,
      unpack_unpremultiply_alpha_needed);

  gl->DeleteTextures(1, &source_texture);

  const GLuint64 fence_sync = gl->InsertFenceSyncCHROMIUM();

  gl->Flush();
  gpu::SyncToken sync_token;
  gl->GenSyncTokenCHROMIUM(fence_sync, sync_token.GetData());
  gl_->WaitSyncTokenCHROMIUM(sync_token.GetData());

  return true;
}
