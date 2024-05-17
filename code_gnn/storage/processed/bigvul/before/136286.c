    CreateFragmentContextsForRepeatingTableSectionInPagedMedia() {
  DCHECK(ObjectIsRepeatingTableSectionInPagedMedia());

  LayoutView* view = object_.View();
  context_.repeating_table_section_bounding_box =
      BoundingBoxInPaginationContainer(object_, *view->Layer());

  auto page_height = view->PageLogicalHeight();
  const auto& bounding_box = context_.repeating_table_section_bounding_box;
  int first_page = floorf(bounding_box.Y() / page_height);
  int last_page = ceilf(bounding_box.MaxY() / page_height) - 1;
  if (first_page >= last_page)
    return;

  context_.fragments.resize(last_page - first_page + 1);
  for (int page = first_page; page <= last_page; page++) {
    if (page > first_page)
      context_.fragments[page - first_page] = context_.fragments[0];
    context_.fragments[page - first_page].logical_top_in_flow_thread =
        page * page_height;
  }

  const auto& painting_object = context_.painting_layer->GetLayoutObject();
  if (painting_object == object_)
    return;

  int page_count = ceilf(view->DocumentRect().Height() / page_height);
  auto* fragment = &painting_object.GetMutableForPainting().FirstFragment();
  if (fragment->NextFragment()) {
#if DCHECK_IS_ON()
    int fragment_count = 1;
    while ((fragment = fragment->NextFragment()))
      fragment_count++;
    DCHECK_EQ(fragment_count, page_count);
#endif
    return;
  }

  for (int page = 1; page < page_count; page++) {
    auto* new_fragment = &fragment->EnsureNextFragment();
    new_fragment->SetLocalBorderBoxProperties(
        fragment->LocalBorderBoxProperties());
    new_fragment->SetLogicalTopInFlowThread(page * page_height);
    fragment = new_fragment;
  }
}
