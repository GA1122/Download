TransformPaintPropertyNode::NearestScrollTranslationNode() const {
  const auto* transform = this;
  while (!transform->ScrollNode()) {
    transform = transform->Parent();
    DCHECK(transform);
  }
  return *transform;
}
