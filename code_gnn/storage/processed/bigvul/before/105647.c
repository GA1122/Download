void TreeView::ExpandAll() {
  DCHECK(model_);
  ExpandAll(model_->GetRoot());
}
