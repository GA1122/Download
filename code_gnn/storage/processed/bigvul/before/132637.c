void BlinkTestRunner::DisableAutoResizeMode(const WebSize& new_size) {
  content::DisableAutoResizeMode(render_view(), new_size);
  if (!new_size.isEmpty())
    ForceResizeRenderView(render_view(), new_size);
}
