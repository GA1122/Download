DOMWindow* LocalDOMWindow::open(const String& url_string,
                                const AtomicString& frame_name,
                                const String& window_features_string,
                                LocalDOMWindow* calling_window,
                                LocalDOMWindow* entered_window,
                                ExceptionState& exception_state) {
  if (!IsCurrentlyDisplayedInFrame())
    return nullptr;
  if (!calling_window->GetFrame())
    return nullptr;
  Document* active_document = calling_window->document();
  if (!active_document)
    return nullptr;
  LocalFrame* first_frame = entered_window->GetFrame();
  if (!first_frame)
    return nullptr;

  UseCounter::Count(*active_document, WebFeature::kDOMWindowOpen);
  if (!window_features_string.IsEmpty())
    UseCounter::Count(*active_document, WebFeature::kDOMWindowOpenFeatures);

  Frame* target_frame = nullptr;
  if (EqualIgnoringASCIICase(frame_name, "_top")) {
    target_frame = &GetFrame()->Tree().Top();
  } else if (EqualIgnoringASCIICase(frame_name, "_parent")) {
    if (Frame* parent = GetFrame()->Tree().Parent())
      target_frame = parent;
    else
      target_frame = GetFrame();
  }

  if (target_frame) {
    if (!active_document->GetFrame() ||
        !active_document->GetFrame()->CanNavigate(*target_frame)) {
      return nullptr;
    }

    KURL completed_url = first_frame->GetDocument()->CompleteURL(url_string);

    if (target_frame->DomWindow()->IsInsecureScriptAccess(*calling_window,
                                                          completed_url))
      return target_frame->DomWindow();

    if (url_string.IsEmpty())
      return target_frame->DomWindow();

    target_frame->Navigate(*active_document, completed_url, false,
                           UserGestureStatus::kNone);
    return target_frame->DomWindow();
  }

  DOMWindow* new_window =
      CreateWindow(url_string, frame_name, window_features_string,
                   *calling_window, *first_frame, *GetFrame(), exception_state);
  return new_window;
}
