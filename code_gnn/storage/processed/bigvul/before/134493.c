void WebContentsViewAura::EndDrag(blink::WebDragOperationsMask ops) {
  aura::Window* root_window = GetNativeView()->GetRootWindow();
  gfx::Point screen_loc =
      gfx::Screen::GetScreenFor(GetNativeView())->GetCursorScreenPoint();
  gfx::Point client_loc = screen_loc;
  RenderViewHost* rvh = web_contents_->GetRenderViewHost();
  aura::Window* window = rvh->GetView()->GetNativeView();
  aura::Window::ConvertPointToTarget(root_window, window, &client_loc);
  if (!web_contents_)
    return;
  web_contents_->DragSourceEndedAt(client_loc.x(), client_loc.y(),
      screen_loc.x(), screen_loc.y(), ops);
}
