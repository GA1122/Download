int LocalDOMWindow::requestAnimationFrame(V8FrameRequestCallback* callback) {
  FrameRequestCallbackCollection::V8FrameCallback* frame_callback =
      FrameRequestCallbackCollection::V8FrameCallback::Create(callback);
  frame_callback->SetUseLegacyTimeBase(false);
  if (Document* doc = document())
    return doc->RequestAnimationFrame(frame_callback);
  return 0;
}
