  void SetFrameOwnerBasedOnFrameType(WebURLRequest::FrameType frame_type,
                                     HTMLIFrameElement* iframe,
                                     const AtomicString& potential_value) {
    if (frame_type != WebURLRequest::kFrameTypeNested) {
      document->GetFrame()->SetOwner(nullptr);
      return;
    }

    iframe->setAttribute(HTMLNames::cspAttr, potential_value);
    document->GetFrame()->SetOwner(iframe);
  }
