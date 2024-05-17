void WebContentsViewAura::OnMouseEvent(ui::MouseEvent* event) {
  if (!web_contents_->GetDelegate())
    return;

  switch (event->type()) {
    case ui::ET_MOUSE_PRESSED:
      web_contents_->GetDelegate()->ActivateContents(web_contents_);
      break;
    case ui::ET_MOUSE_MOVED:
    case ui::ET_MOUSE_EXITED:
      web_contents_->GetDelegate()->ContentsMouseEvent(
          web_contents_,
          gfx::Screen::GetScreenFor(GetNativeView())->GetCursorScreenPoint(),
          event->type() == ui::ET_MOUSE_MOVED);
      break;
    default:
      break;
  }
}
