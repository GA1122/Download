void Layer::RemoveFromScrollTree() {
  if (scroll_children_.get()) {
    for (std::set<Layer*>::iterator it = scroll_children_->begin();
        it != scroll_children_->end(); ++it)
      (*it)->scroll_parent_ = NULL;
  }

  if (scroll_parent_)
    scroll_parent_->RemoveScrollChild(this);

  scroll_parent_ = NULL;
}
