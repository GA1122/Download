void RenderViewImpl::DidPause(webkit_media::WebMediaPlayerImpl* player) {
  Send(new ViewHostMsg_MediaNotification(routing_id_,
                                         reinterpret_cast<int64>(player),
                                         player->hasVideo(),
                                         player->hasAudio(),
                                         false));
}
