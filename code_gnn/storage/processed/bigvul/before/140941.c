Element* Document::ActiveElement() const {
  if (Element* element = AdjustedFocusedElement())
    return element;
  return body();
}
