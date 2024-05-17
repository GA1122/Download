void RenderView::logCrossFramePropertyAccess(WebFrame* frame,
                                             WebFrame* target,
                                             bool cross_origin,
                                             const WebString& property_name,
                                             unsigned long long event_id) {
  FOR_EACH_OBSERVER(
      RenderViewObserver,
      observers_,
      LogCrossFramePropertyAccess(
          frame, target, cross_origin, property_name, event_id));
}
