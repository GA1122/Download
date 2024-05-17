static const String ComputeUniqueSelector(Node* anchor_node) {
  DCHECK(anchor_node);
  if (anchor_node->IsPseudoElement()) {
    return String();
  }

  TRACE_EVENT0("blink", "ScrollAnchor::SerializeAnchor");
  SCOPED_BLINK_UMA_HISTOGRAM_TIMER(
      "Layout.ScrollAnchor.TimeToComputeAnchorNodeSelector");

  Vector<String> selector_list;
  for (Element* element = ElementTraversal::FirstAncestorOrSelf(*anchor_node);
       element; element = ElementTraversal::FirstAncestor(*element)) {
    selector_list.push_back(UniqueSimpleSelectorAmongSiblings(element));
    if (element->HasID() &&
        !element->GetDocument().ContainsMultipleElementsWithId(
            element->GetIdAttribute())) {
      break;
    }
  }

  StringBuilder builder;
  size_t i = 0;
  for (auto reverse_iterator = selector_list.rbegin();
       reverse_iterator != selector_list.rend(); ++reverse_iterator, ++i) {
    if (i)
      builder.Append(">");
    builder.Append(*reverse_iterator);
  }

  DEFINE_STATIC_LOCAL(CustomCountHistogram, selector_length_histogram,
                      ("Layout.ScrollAnchor.SerializedAnchorSelectorLength", 1,
                       kMaxSerializedSelectorLength, 50));
  selector_length_histogram.Count(builder.length());

  if (builder.length() > kMaxSerializedSelectorLength) {
    return String();
  }

  return builder.ToString();
}
