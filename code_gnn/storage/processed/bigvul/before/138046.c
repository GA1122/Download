static Node* getParentNodeForComputeParent(Node* node) {
  if (!node)
    return nullptr;

  Node* parentNode = nullptr;

  if (isHTMLOptionElement(node))
    parentNode = toHTMLOptionElement(node)->ownerSelectElement();

  if (!parentNode)
    parentNode = node->parentNode();

  return parentNode;
}
