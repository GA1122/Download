void GranularityStrategyTest::SetSelection(
    const VisibleSelection& new_selection) {
  dummy_page_holder_->GetFrame().Selection().SetSelection(
      new_selection.AsSelection());
}
