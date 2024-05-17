void RenderFrameImpl::DidPlay(WebMediaPlayer* player) {
  has_played_media_ = true;
  Send(new FrameHostMsg_MediaPlayingNotification(
      routing_id_, reinterpret_cast<int64>(player), player->hasVideo(),
      player->hasAudio(), player->isRemote()));
}
