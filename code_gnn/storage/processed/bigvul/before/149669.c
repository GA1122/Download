static LayoutBox* ScrollerLayoutBox(const ScrollableArea* scroller) {
  LayoutBox* box = scroller->GetLayoutBox();
  DCHECK(box);
  return box;
}
