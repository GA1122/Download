bool RenderWidgetHostImpl::KeyPressListenersHandleEvent(ui::KeyEvent* event) {
  if (event->type() != ui::ET_KEY_PRESSED)
    return false;

  for (std::list<KeyboardListener*>::iterator it = keyboard_listeners_.begin();
       it != keyboard_listeners_.end(); ++it) {
    if ((*it)->HandleKeyPressEvent(event))
      return true;
  }

  return false;
}
