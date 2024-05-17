 ClipPaintPropertyNode* ClipPaintPropertyNode::Root() {
  DEFINE_STATIC_REF(
// const ClipPaintPropertyNode& ClipPaintPropertyNode::Root() {
//   DEFINE_STATIC_LOCAL(
        ClipPaintPropertyNode, root,
      (ClipPaintPropertyNode::Create(
          nullptr, State{TransformPaintPropertyNode::Root(),
                         FloatRoundedRect(LayoutRect::InfiniteIntRect())})));
//       (nullptr, State{&TransformPaintPropertyNode::Root(),
//                       FloatRoundedRect(LayoutRect::InfiniteIntRect())}));
    return root;
  }