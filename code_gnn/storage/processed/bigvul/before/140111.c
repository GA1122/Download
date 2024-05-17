void HTMLMediaElement::setTextTrackKindUserPreferenceForAllMediaElements(
    Document* document) {
  auto it = documentToElementSetMap().find(document);
  if (it == documentToElementSetMap().end())
    return;
  DCHECK(it->value);
  WeakMediaElementSet& elements = *it->value;
  for (const auto& element : elements)
    element->automaticTrackSelectionForUpdatedUserPreference();
}
