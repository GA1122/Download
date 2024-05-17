bool Layer::DescendantIsFixedToContainerLayer() const {
  for (size_t i = 0; i < children_.size(); ++i) {
    if (children_[i]->position_constraint_.is_fixed_position() ||
        children_[i]->DescendantIsFixedToContainerLayer())
      return true;
  }
  return false;
}
