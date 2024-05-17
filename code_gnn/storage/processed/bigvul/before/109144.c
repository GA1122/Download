void RenderViewImpl::printPage(WebFrame* frame) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    PrintPage(frame, handling_input_event_));
}
