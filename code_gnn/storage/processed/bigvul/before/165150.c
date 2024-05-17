const ListedElement::List& HTMLFormElement::ListedElements() const {
  if (!listed_elements_are_dirty_)
    return listed_elements_;
  HTMLFormElement* mutable_this = const_cast<HTMLFormElement*>(this);
  Node* scope = mutable_this;
  if (has_elements_associated_by_parser_)
    scope = &NodeTraversal::HighestAncestorOrSelf(*mutable_this);
  if (isConnected() && has_elements_associated_by_form_attribute_)
    scope = &GetTreeScope().RootNode();
  DCHECK(scope);
  CollectListedElements(*scope, mutable_this->listed_elements_);
  mutable_this->listed_elements_are_dirty_ = false;
  return listed_elements_;
}
