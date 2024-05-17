void HTMLFormElement::CollectImageElements(
    Node& root,
    HeapVector<Member<HTMLImageElement>>& elements) {
  elements.clear();
  for (HTMLImageElement& image :
       Traversal<HTMLImageElement>::StartsAfter(root)) {
    if (image.formOwner() == this)
      elements.push_back(&image);
  }
}
