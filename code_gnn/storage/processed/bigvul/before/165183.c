static DocumentFragment* DocumentFragmentFromDragData(
    DragData* drag_data,
    LocalFrame* frame,
    Range* context,
    bool allow_plain_text,
    DragSourceType& drag_source_type) {
  DCHECK(drag_data);
  drag_source_type = DragSourceType::kHTMLSource;

  Document& document = context->OwnerDocument();
  if (drag_data->ContainsCompatibleContent()) {
    if (DocumentFragment* fragment = drag_data->AsFragment(frame))
      return fragment;

    if (drag_data->ContainsURL(DragData::kDoNotConvertFilenames)) {
      String title;
      String url = drag_data->AsURL(DragData::kDoNotConvertFilenames, &title);
      if (!url.IsEmpty()) {
        HTMLAnchorElement* anchor = HTMLAnchorElement::Create(document);
        anchor->SetHref(AtomicString(url));
        if (title.IsEmpty()) {
          if (drag_data->ContainsPlainText())
            title = drag_data->AsPlainText();
          if (title.IsEmpty())
            title = url;
        }
        Node* anchor_text = document.createTextNode(title);
        anchor->AppendChild(anchor_text);
        DocumentFragment* fragment = document.createDocumentFragment();
        fragment->AppendChild(anchor);
        return fragment;
      }
    }
  }
  if (allow_plain_text && drag_data->ContainsPlainText()) {
    drag_source_type = DragSourceType::kPlainTextSource;
    return CreateFragmentFromText(EphemeralRange(context),
                                  drag_data->AsPlainText());
  }

  return nullptr;
}
