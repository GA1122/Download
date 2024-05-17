void HTMLMediaElement::SelectMediaResource() {
  BLINK_MEDIA_LOG << "selectMediaResource(" << (void*)this << ")";

  enum Mode { kObject, kAttribute, kChildren, kNothing };
  Mode mode = kNothing;

  if (src_object_) {
    mode = kObject;
  } else if (FastHasAttribute(kSrcAttr)) {
    mode = kAttribute;
  } else if (HTMLSourceElement* element =
                 Traversal<HTMLSourceElement>::FirstChild(*this)) {
    mode = kChildren;
    next_child_node_to_consider_ = element;
    current_source_node_ = nullptr;
  } else {
    load_state_ = kWaitingForSource;
    SetShouldDelayLoadEvent(false);
    if (!GetWebMediaPlayer() || (ready_state_ < kHaveFutureData &&
                                 ready_state_maximum_ < kHaveFutureData)) {
      SetNetworkState(kNetworkEmpty);
    } else {
      UseCounter::Count(GetDocument(),
                        WebFeature::kHTMLMediaElementEmptyLoadWithFutureData);
    }
    UpdateDisplayState();

    BLINK_MEDIA_LOG << "selectMediaResource(" << (void*)this
                    << "), nothing to load";
    return;
  }

  SetNetworkState(kNetworkLoading);

  ScheduleEvent(event_type_names::kLoadstart);

  switch (mode) {
    case kObject:
      LoadSourceFromObject();
      BLINK_MEDIA_LOG << "selectMediaResource(" << (void*)this
                      << ", using 'srcObject' attribute";
      break;
    case kAttribute:
      LoadSourceFromAttribute();
      BLINK_MEDIA_LOG << "selectMediaResource(" << (void*)this
                      << "), using 'src' attribute url";
      break;
    case kChildren:
      LoadNextSourceChild();
      BLINK_MEDIA_LOG << "selectMediaResource(" << (void*)this
                      << "), using source element";
      break;
    default:
      NOTREACHED();
  }
}
