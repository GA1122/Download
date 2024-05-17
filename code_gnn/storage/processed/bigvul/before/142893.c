KURL HTMLMediaElement::SelectNextSourceChild(
    String* content_type,
    InvalidURLAction action_if_invalid) {
  bool should_log = action_if_invalid != kDoNothing;
  if (should_log)
    BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this << ")";

  if (!next_child_node_to_consider_) {
    if (should_log) {
      BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this
                      << ") -> 0x0000, \"\"";
    }
    return KURL();
  }

  KURL media_url;
  Node* node;
  HTMLSourceElement* source = nullptr;
  String type;
  bool looking_for_start_node = next_child_node_to_consider_;
  bool can_use_source_element = false;

  NodeVector potential_source_nodes;
  GetChildNodes(*this, potential_source_nodes);

  for (unsigned i = 0;
       !can_use_source_element && i < potential_source_nodes.size(); ++i) {
    node = potential_source_nodes[i].Get();
    if (looking_for_start_node && next_child_node_to_consider_ != node)
      continue;
    looking_for_start_node = false;

    if (!IsHTMLSourceElement(*node))
      continue;
    if (node->parentNode() != this)
      continue;

    source = ToHTMLSourceElement(node);

    const AtomicString& src_value = source->FastGetAttribute(kSrcAttr);
    if (should_log) {
      BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this
                      << ") - 'src' is " << UrlForLoggingMedia(media_url);
    }
    if (src_value.IsEmpty())
      goto checkAgain;

    media_url = source->GetDocument().CompleteURL(src_value);

    if (!IsSafeToLoadURL(media_url, action_if_invalid))
      goto checkAgain;

    type = source->type();
    if (type.IsEmpty() && media_url.ProtocolIsData())
      type = MimeTypeFromDataURL(media_url);
    if (!type.IsEmpty()) {
      if (should_log) {
        BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this
                        << ") - 'type' is '" << type << "'";
      }
      if (!GetSupportsType(ContentType(type)))
        goto checkAgain;
    }

    can_use_source_element = true;

  checkAgain:
    if (!can_use_source_element && action_if_invalid == kComplain && source)
      source->ScheduleErrorEvent();
  }

  if (can_use_source_element) {
    if (content_type)
      *content_type = type;
    current_source_node_ = source;
    next_child_node_to_consider_ = source->nextSibling();
  } else {
    current_source_node_ = nullptr;
    next_child_node_to_consider_ = nullptr;
  }

  if (should_log) {
    BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this << ") -> "
                    << current_source_node_.Get() << ", "
                    << (can_use_source_element ? UrlForLoggingMedia(media_url)
                                               : "");
  }

  return can_use_source_element ? media_url : KURL();
}
