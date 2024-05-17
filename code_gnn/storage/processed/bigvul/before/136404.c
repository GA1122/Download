static const EffectPaintPropertyNode* StrictChildOfAlongPath(
    const EffectPaintPropertyNode* ancestor,
    const EffectPaintPropertyNode* node) {
  for (; node; node = node->Parent()) {
    if (node->Parent() == ancestor)
      return node;
  }
  return nullptr;
}
