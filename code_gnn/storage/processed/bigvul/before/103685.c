void BrowserRenderProcessHost::SetCompositingSurface(
    int render_widget_id,
    gfx::PluginWindowHandle compositing_surface) {
  widget_helper_->SetCompositingSurface(render_widget_id, compositing_surface);
}
