void PaintPropertyTreeBuilder::CreateFragmentDataForRepeatingInPagedMedia(
    bool needs_paint_properties) {
  DCHECK(IsRepeatingInPagedMedia());

  FragmentData* fragment_data = nullptr;
  for (auto& fragment_context : context_.fragments) {
    fragment_data = fragment_data
                        ? &fragment_data->EnsureNextFragment()
                        : &object_.GetMutableForPainting().FirstFragment();
    InitFragmentPaintProperties(*fragment_data, needs_paint_properties,
                                LayoutPoint(),
                                fragment_context.logical_top_in_flow_thread);
  }
  DCHECK(fragment_data);
  fragment_data->ClearNextFragment();
}
