void RenderWidgetHostViewAura::OnMouseEvent(ui::MouseEvent* event) {
  TRACE_EVENT0("input", "RenderWidgetHostViewAura::OnMouseEvent");

  if (touch_editing_client_ && touch_editing_client_->HandleInputEvent(event))
    return;

  if (mouse_locked_) {
    aura::client::CursorClient* cursor_client =
        aura::client::GetCursorClient(window_->GetRootWindow());
    DCHECK(!cursor_client || !cursor_client->IsCursorVisible());

    if (event->type() == ui::ET_MOUSEWHEEL) {
      blink::WebMouseWheelEvent mouse_wheel_event =
          MakeWebMouseWheelEvent(static_cast<ui::MouseWheelEvent*>(event));
      if (mouse_wheel_event.deltaX != 0 || mouse_wheel_event.deltaY != 0)
        host_->ForwardWheelEvent(mouse_wheel_event);
      return;
    }

    gfx::Point center(gfx::Rect(window_->bounds().size()).CenterPoint());

    if (event->flags() & ui::EF_IS_NON_CLIENT) {
      synthetic_move_sent_ = true;
      window_->MoveCursorTo(center);
      return;
    }

    blink::WebMouseEvent mouse_event = MakeWebMouseEvent(event);

    bool is_move_to_center_event = (event->type() == ui::ET_MOUSE_MOVED ||
        event->type() == ui::ET_MOUSE_DRAGGED) &&
        mouse_event.x == center.x() && mouse_event.y == center.y();

    ModifyEventMovementAndCoords(&mouse_event);

    bool should_not_forward = is_move_to_center_event && synthetic_move_sent_;
    if (should_not_forward) {
      synthetic_move_sent_ = false;
    } else {
      if (ShouldMoveToCenter()) {
        synthetic_move_sent_ = true;
        window_->MoveCursorTo(center);
      }
      if (CanRendererHandleEvent(event) &&
          !(event->flags() & ui::EF_FROM_TOUCH)) {
        host_->ForwardMouseEvent(mouse_event);
        if (event->type() == ui::ET_MOUSE_PRESSED)
          SetKeyboardFocus();
      }
    }
    return;
  }

  if (host_->overscroll_controller() &&
      host_->overscroll_controller()->overscroll_mode() != OVERSCROLL_NONE &&
      event->flags() & ui::EF_IS_SYNTHESIZED &&
      (event->type() == ui::ET_MOUSE_ENTERED ||
       event->type() == ui::ET_MOUSE_EXITED ||
       event->type() == ui::ET_MOUSE_MOVED)) {
    event->StopPropagation();
    return;
  }

  if (event->type() == ui::ET_MOUSEWHEEL) {
#if defined(OS_WIN)
    aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
    if (dispatcher) {
      HWND parent = dispatcher->host()->GetAcceleratedWidget();
      HWND toplevel_hwnd = ::GetAncestor(parent, GA_ROOT);
      EnumThreadWindows(GetCurrentThreadId(),
                        DismissOwnedPopups,
                        reinterpret_cast<LPARAM>(toplevel_hwnd));
    }
#endif
    blink::WebMouseWheelEvent mouse_wheel_event =
        MakeWebMouseWheelEvent(static_cast<ui::MouseWheelEvent*>(event));
    if (mouse_wheel_event.deltaX != 0 || mouse_wheel_event.deltaY != 0)
      host_->ForwardWheelEvent(mouse_wheel_event);
  } else if (CanRendererHandleEvent(event) &&
             !(event->flags() & ui::EF_FROM_TOUCH)) {
    blink::WebMouseEvent mouse_event = MakeWebMouseEvent(event);
    ModifyEventMovementAndCoords(&mouse_event);
    host_->ForwardMouseEvent(mouse_event);
    if (event->type() == ui::ET_MOUSE_PRESSED)
      SetKeyboardFocus();
  }

  switch (event->type()) {
    case ui::ET_MOUSE_PRESSED:
      window_->SetCapture();
      FinishImeCompositionSession();
      break;
    case ui::ET_MOUSE_RELEASED:
      window_->ReleaseCapture();
      break;
    default:
      break;
  }

  if (!is_fullscreen_ && window_->parent()->delegate() &&
      !(event->flags() & ui::EF_FROM_TOUCH))
    window_->parent()->delegate()->OnMouseEvent(event);

  if (!IsXButtonUpEvent(event))
    event->SetHandled();
}
