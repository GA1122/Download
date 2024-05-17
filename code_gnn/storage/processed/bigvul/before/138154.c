HTMLDialogElement* getActiveDialogElement(Node* node) {
  return node->document().activeModalDialog();
}
