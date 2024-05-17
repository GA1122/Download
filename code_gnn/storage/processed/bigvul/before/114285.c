bool WebGraphicsContext3DCommandBufferImpl::SetParent(
    WebGraphicsContext3DCommandBufferImpl* new_parent) {
  if (parent_ == new_parent)
    return true;

  uint32 new_parent_texture_id = 0;
  if (command_buffer_) {
    if (new_parent) {
      int32 token = new_parent->gles2_helper_->InsertToken();
      new_parent->gles2_helper_->WaitForToken(token);
      new_parent_texture_id =
        new_parent->gl_->MakeTextureId();

      if (!command_buffer_->SetParent(new_parent->command_buffer_,
                                      new_parent_texture_id)) {
        new_parent->gl_->FreeTextureId(parent_texture_id_);
        return false;
      }
    } else {
      if (!command_buffer_->SetParent(NULL, 0))
        return false;
    }
  }

  if (parent_ && parent_texture_id_ != 0) {
    gpu::gles2::GLES2Implementation* parent_gles2 =
        parent_->gl_;
    parent_gles2->helper()->CommandBufferHelper::Finish();
    parent_gles2->FreeTextureId(parent_texture_id_);
  }

  if (new_parent) {
    parent_ = new_parent;
    parent_texture_id_ = new_parent_texture_id;
  } else {
    parent_ = NULL;
    parent_texture_id_ = 0;
  }

  return true;
}
