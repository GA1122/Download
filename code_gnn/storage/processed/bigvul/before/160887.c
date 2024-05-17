bool WebViewTest::TapElement(WebInputEvent::Type type, Element* element) {
  if (!element || !element->GetLayoutObject())
    return false;

  DCHECK(web_view_helper_.GetWebView());
  element->scrollIntoViewIfNeeded();

  IntPoint center =
      web_view_helper_.GetWebView()
          ->MainFrameImpl()
          ->GetFrameView()
          ->ContentsToScreen(
              element->GetLayoutObject()->AbsoluteBoundingBoxRect())
          .Center();

  WebGestureEvent event(type, WebInputEvent::kNoModifiers,
                        WebInputEvent::kTimeStampForTesting);

  event.source_device = kWebGestureDeviceTouchscreen;
  event.x = center.X();
  event.y = center.Y();

  web_view_helper_.GetWebView()->HandleInputEvent(
      WebCoalescedInputEvent(event));
  RunPendingTasks();
  return true;
}
