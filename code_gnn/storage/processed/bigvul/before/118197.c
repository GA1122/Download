bool AutofillDialogViews::SuggestionView::CanUseVerticallyCompactText(
    int available_width,
    int* resulting_height) const {
  if (!calculated_heights_.count(available_width)) {
    SuggestionView sizing_view(NULL);
    sizing_view.SetLabelText(state_.vertically_compact_text);
    sizing_view.SetIcon(state_.icon);
    sizing_view.SetTextfield(state_.extra_text, state_.extra_icon);
    sizing_view.label_->SetSize(gfx::Size(available_width, 0));
    sizing_view.label_line_2_->SetSize(gfx::Size(available_width, 0));

    views::LayoutManager* layout = sizing_view.GetLayoutManager();
    if (layout->GetPreferredSize(&sizing_view).width() <= available_width) {
      calculated_heights_[available_width] = std::make_pair(
          true,
          layout->GetPreferredHeightForWidth(&sizing_view, available_width));
    } else {
      sizing_view.SetLabelText(state_.horizontally_compact_text);
      calculated_heights_[available_width] = std::make_pair(
          false,
          layout->GetPreferredHeightForWidth(&sizing_view, available_width));
    }
  }

  const std::pair<bool, int>& values = calculated_heights_[available_width];
  *resulting_height = values.second;
  return values.first;
}
