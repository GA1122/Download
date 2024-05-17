void Document::SetReadyState(DocumentReadyState ready_state) {
  if (ready_state == ready_state_)
    return;

  switch (ready_state) {
    case kLoading:
      if (document_timing_.DomLoading().is_null()) {
        document_timing_.MarkDomLoading();
      }
      break;
    case kInteractive:
      if (document_timing_.DomInteractive().is_null())
        document_timing_.MarkDomInteractive();
      break;
    case kComplete:
      if (document_timing_.DomComplete().is_null())
        document_timing_.MarkDomComplete();
      break;
  }

  ready_state_ = ready_state;
  DispatchEvent(*Event::Create(event_type_names::kReadystatechange));
}
