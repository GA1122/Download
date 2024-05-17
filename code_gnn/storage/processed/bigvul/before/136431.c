static ScrollPaintPropertyNode::State ScrollState1() {
  ScrollPaintPropertyNode::State state;
  state.container_rect = IntRect(3, 5, 11, 13);
  state.contents_rect = IntRect(-3, -5, 27, 31);
  state.user_scrollable_horizontal = true;
  return state;
}
