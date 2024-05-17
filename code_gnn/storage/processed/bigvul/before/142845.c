void HTMLMediaElement::OnMediaControlsEnabledChange(Document* document) {
  auto it = DocumentToElementSetMap().find(document);
  if (it == DocumentToElementSetMap().end())
    return;
  DCHECK(it->value);
  WeakMediaElementSet& elements = *it->value;
  for (const auto& element : elements) {
    element->UpdateControlsVisibility();
    if (element->GetMediaControls())
      element->GetMediaControls()->OnMediaControlsEnabledChange();
  }
}
