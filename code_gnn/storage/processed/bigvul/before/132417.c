bool MoveCursor(int swipe_direction,
                int modifier_flags,
                aura::WindowTreeHost* host) {
  if (!host)
    return false;
  ui::DomCode domcodex = ui::DomCode::NONE;
  ui::DomCode domcodey = ui::DomCode::NONE;
  if (swipe_direction & kCursorMoveRight)
    domcodex = ui::DomCode::ARROW_RIGHT;
  else if (swipe_direction & kCursorMoveLeft)
    domcodex = ui::DomCode::ARROW_LEFT;

  if (swipe_direction & kCursorMoveUp)
    domcodey = ui::DomCode::ARROW_UP;
  else if (swipe_direction & kCursorMoveDown)
    domcodey = ui::DomCode::ARROW_DOWN;

  if (domcodex != ui::DomCode::NONE) {
    ui::KeyboardCode codex = ui::VKEY_UNKNOWN;
    ui::DomKey domkeyx = ui::DomKey::NONE;
    base::char16 cx;
    ignore_result(DomCodeToUsLayoutMeaning(domcodex, ui::EF_NONE, &domkeyx,
                                           &cx, &codex));
    ui::KeyEvent press_event(ui::ET_KEY_PRESSED, codex, domcodex,
                             modifier_flags, domkeyx, cx,
                             ui::EventTimeForNow());
    ui::EventDispatchDetails details =
        host->event_processor()->OnEventFromSource(&press_event);
    CHECK(!details.dispatcher_destroyed);
    ui::KeyEvent release_event(ui::ET_KEY_RELEASED, codex, domcodex,
                               modifier_flags, domkeyx, cx,
                               ui::EventTimeForNow());
    details = host->event_processor()->OnEventFromSource(&release_event);
    CHECK(!details.dispatcher_destroyed);
  }

  if (domcodey != ui::DomCode::NONE) {
    ui::KeyboardCode codey = ui::VKEY_UNKNOWN;
    ui::DomKey domkeyy = ui::DomKey::NONE;
    base::char16 cy;
    ignore_result(DomCodeToUsLayoutMeaning(domcodey, ui::EF_NONE, &domkeyy,
                                           &cy, &codey));
    ui::KeyEvent press_event(ui::ET_KEY_PRESSED, codey, domcodey,
                             modifier_flags, domkeyy, cy,
                             ui::EventTimeForNow());
    ui::EventDispatchDetails details =
        host->event_processor()->OnEventFromSource(&press_event);
    CHECK(!details.dispatcher_destroyed);
    ui::KeyEvent release_event(ui::ET_KEY_RELEASED, codey, domcodey,
                               modifier_flags, domkeyy, cy,
                               ui::EventTimeForNow());
    details = host->event_processor()->OnEventFromSource(&release_event);
    CHECK(!details.dispatcher_destroyed);
  }
  return true;
}
