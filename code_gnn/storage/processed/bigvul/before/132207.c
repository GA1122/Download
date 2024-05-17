RendererMediaPlayerManager* RenderFrameImpl::GetMediaPlayerManager() {
  if (!media_player_manager_)
    media_player_manager_ = new RendererMediaPlayerManager(this);
  return media_player_manager_;
}
