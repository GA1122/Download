blink::WebFrame* RenderFrameImpl::FindFrame(const blink::WebString& name) {
  if (render_view_->renderer_wide_named_frame_lookup()) {
    for (const auto& it : g_routing_id_frame_map.Get()) {
      WebLocalFrame* frame = it.second->GetWebFrame();
      if (frame->AssignedName() == name)
        return frame;
    }
  }

  return GetContentClient()->renderer()->FindFrame(this->GetWebFrame(),
                                                   name.Utf8());
}
