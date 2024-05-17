void HTMLFormElement::removeFromPastNamesMap(HTMLElement& element) {
  if (!m_pastNamesMap)
    return;
  for (auto& it : *m_pastNamesMap) {
    if (it.value == &element) {
      it.value = nullptr;
    }
  }
}
