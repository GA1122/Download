void HTMLInputElement::AttachLayoutTree(AttachContext& context) {
  SyncReattachContext reattach_context(context);
  TextControlElement::AttachLayoutTree(context);
  if (GetLayoutObject()) {
    input_type_->OnAttachWithLayoutObject();
  }

  input_type_view_->StartResourceLoading();
  input_type_->CountUsage();
}
