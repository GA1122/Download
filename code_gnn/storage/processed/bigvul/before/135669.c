String FrameSelection::SelectedHTMLForClipboard() const {
  const VisibleSelectionInFlatTree& visible_selection =
      ComputeVisibleSelectionInFlatTree();
  const EphemeralRangeInFlatTree& range =
      visible_selection.ToNormalizedEphemeralRange();
  return CreateMarkup(
      range.StartPosition(), range.EndPosition(), kAnnotateForInterchange,
      ConvertBlocksToInlines::kNotConvert, kResolveNonLocalURLs);
}
