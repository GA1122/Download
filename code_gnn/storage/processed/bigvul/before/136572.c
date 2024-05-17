bool ScrollHitTestDisplayItem::Equals(const DisplayItem& other) const {
  return DisplayItem::Equals(other) &&
         &scroll_node() ==
             &static_cast<const ScrollHitTestDisplayItem&>(other).scroll_node();
}
