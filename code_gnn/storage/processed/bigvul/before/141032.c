void Document::DispatchEventsForPrinting() {
  if (!scripted_animation_controller_)
    return;
  scripted_animation_controller_->DispatchEventsAndCallbacksForPrinting();
}
