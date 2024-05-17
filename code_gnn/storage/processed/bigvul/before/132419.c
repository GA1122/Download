void SendProcessKeyEvent(ui::EventType type,
                         aura::WindowTreeHost* host) {
  ui::KeyEvent event(type, ui::VKEY_PROCESSKEY, ui::DomCode::NONE, ui::EF_NONE,
                     ui::DomKey::PROCESS, 0, ui::EventTimeForNow());
  event.SetTranslated(true);
  ui::EventDispatchDetails details =
      host->event_processor()->OnEventFromSource(&event);
  CHECK(!details.dispatcher_destroyed);
}
