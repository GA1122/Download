void PresentationConnection::dispatchEventAsync(EventTarget* target,
                                                Event* event) {
  DCHECK(target);
  DCHECK(event);
  target->dispatchEvent(event);
}
