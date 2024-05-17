const RefCountedPropertyTreeState& RefCountedPropertyTreeState::Root() {
  DEFINE_STATIC_LOCAL(
      std::unique_ptr<RefCountedPropertyTreeState>, root,
      (std::make_unique<RefCountedPropertyTreeState>(
          TransformPaintPropertyNode::Root(), ClipPaintPropertyNode::Root(),
          EffectPaintPropertyNode::Root())));
  return *root;
}
