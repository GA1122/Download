void HTMLFormElement::collectImageElements(
    Node& root,
    HeapVector<Member<HTMLImageElement>>& elements) {
  elements.clear();
  for (HTMLImageElement& image :
       Traversal<HTMLImageElement>::startsAfter(root)) {
    if (image.formOwner() == this)
      elements.append(&image);
  }
}
