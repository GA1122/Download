void RenderWidgetHostViewAura::ReturnSubscriberTexture(
    base::WeakPtr<RenderWidgetHostViewAura> rwhva,
    scoped_refptr<OwnedMailbox> subscriber_texture,
    uint32 sync_point) {
  if (!subscriber_texture.get())
    return;
  if (!rwhva)
    return;
  DCHECK_NE(
      rwhva->active_frame_subscriber_textures_.count(subscriber_texture.get()),
      0u);

  subscriber_texture->UpdateSyncPoint(sync_point);

  rwhva->active_frame_subscriber_textures_.erase(subscriber_texture.get());
  if (rwhva->frame_subscriber_ && subscriber_texture->texture_id())
    rwhva->idle_frame_subscriber_textures_.push_back(subscriber_texture);
}
