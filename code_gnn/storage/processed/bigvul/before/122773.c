void BrowserPluginGuest::ShowWidget(RenderViewHost* render_view_host,
                                    int route_id,
                                    const gfx::Rect& initial_pos) {
  gfx::Rect screen_pos(initial_pos);
  screen_pos.Offset(guest_screen_rect_.OffsetFromOrigin());
  static_cast<WebContentsImpl*>(web_contents())->ShowCreatedWidget(route_id,
                                                                   screen_pos);
}
