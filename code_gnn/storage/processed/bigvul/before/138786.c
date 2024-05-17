void RenderFrameHostImpl::RequestSmartClipExtract(SmartClipCallback callback,
                                                  gfx::Rect rect) {
  static uint32_t next_id = 1;
  uint32_t key = next_id++;
  Send(new FrameMsg_ExtractSmartClipData(routing_id_, key, rect));
  smart_clip_callbacks_.insert(std::make_pair(key, callback));
}
