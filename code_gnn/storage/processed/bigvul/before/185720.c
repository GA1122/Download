 ScrollPaintPropertyNode* ScrollPaintPropertyNode::Root() {
  DEFINE_STATIC_REF(ScrollPaintPropertyNode, root,
                    (ScrollPaintPropertyNode::Create(nullptr, State{})));
// const ScrollPaintPropertyNode& ScrollPaintPropertyNode::Root() {
//   DEFINE_STATIC_LOCAL(ScrollPaintPropertyNode, root, (nullptr, State{}));
    return root;
  }