static bool IsBackfaceHidden(const TransformPaintPropertyNode* node) {
  while (node && node->GetBackfaceVisibility() ==
                     TransformPaintPropertyNode::BackfaceVisibility::kInherited)
    node = node->Parent();
  return node && node->GetBackfaceVisibility() ==
                     TransformPaintPropertyNode::BackfaceVisibility::kHidden;
}
