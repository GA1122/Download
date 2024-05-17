bool GLES2DecoderImpl::SetParent(GLES2Decoder* new_parent,
                                 uint32 new_parent_texture_id) {
  if (!offscreen_saved_color_texture_.get())
    return false;

  if (parent_) {
    ChildList::iterator it = std::find(
        parent_->children_.begin(),
        parent_->children_.end(),
        this);
    DCHECK(it != parent_->children_.end());
    parent_->children_.erase(it);
    GLuint service_id = offscreen_saved_color_texture_->id();
    GLuint client_id = 0;
    if (parent_->texture_manager()->GetClientId(service_id, &client_id)) {
      parent_->texture_manager()->RemoveTextureInfo(client_id);
    }
  }

  GLES2DecoderImpl* new_parent_impl = static_cast<GLES2DecoderImpl*>(
      new_parent);
  if (new_parent_impl) {
#ifndef NDEBUG
    ChildList::iterator it = std::find(
        new_parent_impl->children_.begin(),
        new_parent_impl->children_.end(),
        this);
    DCHECK(it == new_parent_impl->children_.end());
#endif
    new_parent_impl->children_.push_back(this);
    GLuint service_id = offscreen_saved_color_texture_->id();

    if (new_parent_impl->texture_manager()->GetTextureInfo(
        new_parent_texture_id))
      new_parent_impl->texture_manager()->RemoveTextureInfo(
          new_parent_texture_id);

    offscreen_saved_color_texture_info_ =
        new_parent_impl->CreateTextureInfo(new_parent_texture_id, service_id);
    offscreen_saved_color_texture_info_->SetNotOwned();
    new_parent_impl->texture_manager()->
       SetInfoTarget(offscreen_saved_color_texture_info_, GL_TEXTURE_2D);

    parent_ = new_parent_impl->AsWeakPtr();

    UpdateParentTextureInfo();
  } else {
    parent_.reset();
    offscreen_saved_color_texture_info_ = NULL;
  }

  return true;
}
