void HTMLFormElement::removedFrom(ContainerNode* insertionPoint) {
  if (m_hasElementsAssociatedByParser) {
    Node& root = NodeTraversal::highestAncestorOrSelf(*this);
    if (!m_associatedElementsAreDirty) {
      FormAssociatedElement::List elements(associatedElements());
      notifyFormRemovedFromTree(elements, root);
    } else {
      FormAssociatedElement::List elements;
      collectAssociatedElements(
          NodeTraversal::highestAncestorOrSelf(*insertionPoint), elements);
      notifyFormRemovedFromTree(elements, root);
      collectAssociatedElements(root, elements);
      notifyFormRemovedFromTree(elements, root);
    }

    if (!m_imageElementsAreDirty) {
      HeapVector<Member<HTMLImageElement>> images(imageElements());
      notifyFormRemovedFromTree(images, root);
    } else {
      HeapVector<Member<HTMLImageElement>> images;
      collectImageElements(
          NodeTraversal::highestAncestorOrSelf(*insertionPoint), images);
      notifyFormRemovedFromTree(images, root);
      collectImageElements(root, images);
      notifyFormRemovedFromTree(images, root);
    }
  }
  document().formController().willDeleteForm(this);
  HTMLElement::removedFrom(insertionPoint);
}
