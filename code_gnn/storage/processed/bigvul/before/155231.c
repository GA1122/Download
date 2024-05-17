unsigned HTMLFormElement::length() const {
  const FormAssociatedElement::List& elements = associatedElements();
  unsigned len = 0;
  for (unsigned i = 0; i < elements.size(); ++i) {
    if (elements[i]->isEnumeratable())
      ++len;
  }
  return len;
}
