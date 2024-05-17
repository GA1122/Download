void TabStrip::UpdateIdealBounds() {
  if (tab_count() == 0)
    return;   

  if (!touch_layout_) {
    std::map<int, TabGroupHeader*> group_headers;
    for (const auto& header_pair : group_headers_)
      group_headers[header_pair.first] = header_pair.second.get();

    const int available_width = (available_width_for_tabs_ < 0)
                                    ? GetTabAreaWidth()
                                    : available_width_for_tabs_;
    layout_helper_->UpdateIdealBounds(
        controller(), &tabs_, std::move(group_headers), available_width);
  }

  new_tab_button_bounds_.set_origin(gfx::Point(GetNewTabButtonIdealX(), 0));
}
