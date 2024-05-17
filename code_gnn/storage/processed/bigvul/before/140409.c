bool TypingCommand::makeEditableRootEmpty(EditingState* editingState) {
  Element* root = endingSelection().rootEditableElement();
  if (!root || !root->hasChildren())
    return false;

  if (root->firstChild() == root->lastChild()) {
    if (isHTMLBRElement(root->firstChild())) {
      if (root->layoutObject() && root->layoutObject()->isLayoutBlockFlow())
        return false;
    }
  }

  while (Node* child = root->firstChild()) {
    removeNode(child, editingState);
    if (editingState->isAborted())
      return false;
  }

  addBlockPlaceholderIfNeeded(root, editingState);
  if (editingState->isAborted())
    return false;
  setEndingSelection(SelectionInDOMTree::Builder()
                         .collapse(Position::firstPositionInNode(root))
                         .setIsDirectional(endingSelection().isDirectional())
                         .build());

  return true;
}
