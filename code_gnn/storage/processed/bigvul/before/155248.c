void HTMLFormElement::submitDialog(FormSubmission* formSubmission) {
  for (Node* node = this; node; node = node->parentOrShadowHostNode()) {
    if (isHTMLDialogElement(*node)) {
      toHTMLDialogElement(*node).closeDialog(formSubmission->result());
      return;
    }
  }
}
