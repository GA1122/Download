std::pair<unsigned, unsigned> FrameSelection::LayoutSelectionStartEndForNG(
    const NGPhysicalTextFragment& text_fragment) const {
  return layout_selection_->SelectionStartEndForNG(text_fragment);
}
