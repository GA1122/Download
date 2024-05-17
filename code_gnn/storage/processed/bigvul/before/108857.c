void RenderViewImpl::DidPause(WebKit::WebMediaPlayer* player) {
  Send(new ViewHostMsg_MediaNotification(routing_id_,
                                         reinterpret_cast<int64>(player),
                                         player->hasVideo(),
                                         player->hasAudio(),
                                         false));
}
