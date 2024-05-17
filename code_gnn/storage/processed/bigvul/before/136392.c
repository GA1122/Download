static bool IsNonCompositingAncestorOf(
    const TransformPaintPropertyNode* ancestor,
    const TransformPaintPropertyNode* node) {
  for (; node != ancestor; node = node->Parent()) {
    if (!node || node->HasDirectCompositingReasons())
      return false;
  }
  return true;
}
