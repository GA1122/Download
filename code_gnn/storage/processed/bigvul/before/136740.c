DispatchEventResult LocalDOMWindow::DispatchEvent(Event* event,
                                                  EventTarget* target) {
#if DCHECK_IS_ON()
  DCHECK(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif

  event->SetTrusted(true);
  event->SetTarget(target ? target : this);
  event->SetCurrentTarget(this);
  event->SetEventPhase(Event::kAtTarget);

  TRACE_EVENT1("devtools.timeline", "EventDispatch", "data",
               InspectorEventDispatchEvent::Data(*event));
  return FireEventListeners(event);
}
