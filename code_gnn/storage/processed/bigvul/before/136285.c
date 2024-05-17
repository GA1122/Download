    CreateFragmentContextsForRepeatingFixedPosition() {
  DCHECK(object_.IsFixedPositionObjectInPagedMedia());

  LayoutView* view = object_.View();
  auto page_height = view->PageLogicalHeight();
  int page_count = ceilf(view->DocumentRect().Height() / page_height);
  context_.fragments.resize(page_count);

  context_.fragments[0].fixed_position.paint_offset.Move(LayoutUnit(),
                                                         -view->ScrollTop());
  for (int page = 1; page < page_count; page++) {
    context_.fragments[page] = context_.fragments[page - 1];
    context_.fragments[page].fixed_position.paint_offset.Move(LayoutUnit(),
                                                              page_height);
    context_.fragments[page].logical_top_in_flow_thread += page_height;
  }
}
