bool IsMotionEvent(XEvent* event) {
  int type = event->type;
  if (type == GenericEvent)
    type = event->xgeneric.evtype;
  return type == MotionNotify;
}
