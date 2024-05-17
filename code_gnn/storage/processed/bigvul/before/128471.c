void ShellSurface::AttemptToStartDrag(int component) {
  DCHECK(widget_);

  if (resizer_)
    return;

  if (widget_->GetNativeWindow()->HasCapture())
    return;

  aura::Window* root_window = widget_->GetNativeWindow()->GetRootWindow();
  gfx::Point drag_location =
      root_window->GetHost()->dispatcher()->GetLastMouseLocationInRoot();
  aura::Window::ConvertPointToTarget(
      root_window, widget_->GetNativeWindow()->parent(), &drag_location);

  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(root_window);
  DCHECK(cursor_client);

  switch (component) {
    case HTCAPTION:
      cursor_client->SetCursor(ui::kCursorPointer);
      break;
    case HTTOP:
      cursor_client->SetCursor(ui::kCursorNorthResize);
      break;
    case HTTOPRIGHT:
      cursor_client->SetCursor(ui::kCursorNorthEastResize);
      break;
    case HTRIGHT:
      cursor_client->SetCursor(ui::kCursorEastResize);
      break;
    case HTBOTTOMRIGHT:
      cursor_client->SetCursor(ui::kCursorSouthEastResize);
      break;
    case HTBOTTOM:
      cursor_client->SetCursor(ui::kCursorSouthResize);
      break;
    case HTBOTTOMLEFT:
      cursor_client->SetCursor(ui::kCursorSouthWestResize);
      break;
    case HTLEFT:
      cursor_client->SetCursor(ui::kCursorWestResize);
      break;
    case HTTOPLEFT:
      cursor_client->SetCursor(ui::kCursorNorthWestResize);
      break;
    default:
      NOTREACHED();
      break;
  }

  resizer_ = ash::CreateWindowResizer(
      ash::WmWindowAura::Get(widget_->GetNativeWindow()), drag_location,
      component, aura::client::WINDOW_MOVE_SOURCE_MOUSE);
  if (!resizer_)
    return;

  origin_ += pending_origin_offset_;
  pending_origin_offset_ = gfx::Vector2d();
  resize_component_ = pending_resize_component_;

  ash::Shell::GetInstance()->AddPreTargetHandler(this);
  widget_->GetNativeWindow()->SetCapture();

  if (IsResizing())
    Configure();
}
