void HTMLMediaElement::ScheduleEvent(Event* event) {
#if LOG_MEDIA_EVENTS
  BLINK_MEDIA_LOG << "ScheduleEvent(" << (void*)this << ")"
                  << " - scheduling '" << event->type() << "'";
#endif
  async_event_queue_->EnqueueEvent(FROM_HERE, *event);
}
