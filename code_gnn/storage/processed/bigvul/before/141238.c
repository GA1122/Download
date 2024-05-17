void Document::SetSequentialFocusNavigationStartingPoint(Node* node) {
  if (!frame_)
    return;
  if (!node) {
    sequential_focus_navigation_starting_point_ = nullptr;
    return;
  }
  DCHECK_EQ(node->GetDocument(), this);
  if (!sequential_focus_navigation_starting_point_)
    sequential_focus_navigation_starting_point_ = Range::Create(*this);
  sequential_focus_navigation_starting_point_->selectNodeContents(
      node, ASSERT_NO_EXCEPTION);
}
