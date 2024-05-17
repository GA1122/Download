bool RenderWidgetHostImpl::KeyPressListenersHandleEvent(GdkEventKey* event) {
  if (event->type != GDK_KEY_PRESS)
    return false;

  for (std::list<KeyboardListener*>::iterator it = keyboard_listeners_.begin();
       it != keyboard_listeners_.end(); ++it) {
    if ((*it)->HandleKeyPressEvent(event))
      return true;
  }

  return false;
}
