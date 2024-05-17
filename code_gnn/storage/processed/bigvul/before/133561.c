void WebContentsImpl::ForEachFrame(
    const base::Callback<void(RenderFrameHost*)>& on_frame) {
  frame_tree_.ForEach(base::Bind(&ForEachFrameInternal, on_frame));
}
