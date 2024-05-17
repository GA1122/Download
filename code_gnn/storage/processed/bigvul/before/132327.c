void RenderFrameImpl::frameDetached(blink::WebFrame* frame, DetachType type) {
  CHECK(!is_detaching_);
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, FrameDetached());
  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    FrameDetached(frame));

  if (type == DetachType::Remove)
    Send(new FrameHostMsg_Detach(routing_id_));

  is_detaching_ = true;

  if (render_widget_) {
    render_widget_->UnregisterRenderFrame(this);
    render_widget_->CloseForFrame();
  }

  FrameMap::iterator it = g_frame_map.Get().find(frame);
  CHECK(it != g_frame_map.Get().end());
  CHECK_EQ(it->second, this);
  g_frame_map.Get().erase(it);

  if (is_subframe_ && type == DetachType::Remove)
    frame->parent()->removeChild(frame);

  frame->close();
  frame_ = nullptr;

  delete this;
}
