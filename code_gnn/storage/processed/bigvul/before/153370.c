void TabStrip::ButtonPressed(views::Button* sender, const ui::Event& event) {
  if (sender == new_tab_button_) {
    base::RecordAction(base::UserMetricsAction("NewTab_Button"));
    UMA_HISTOGRAM_ENUMERATION("Tab.NewTab", TabStripModel::NEW_TAB_BUTTON,
                              TabStripModel::NEW_TAB_ENUM_COUNT);
    if (event.IsMouseEvent()) {
      const ui::MouseEvent& mouse = static_cast<const ui::MouseEvent&>(event);
      if (mouse.IsOnlyMiddleMouseButton()) {
        if (ui::Clipboard::IsSupportedClipboardType(
                ui::CLIPBOARD_TYPE_SELECTION)) {
          ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
          CHECK(clipboard);
          base::string16 clipboard_text;
          clipboard->ReadText(ui::CLIPBOARD_TYPE_SELECTION, &clipboard_text);
          if (!clipboard_text.empty())
            controller_->CreateNewTabWithLocation(clipboard_text);
        }
        return;
      }
    }

    controller_->CreateNewTab();
    if (event.type() == ui::ET_GESTURE_TAP)
      TouchUMA::RecordGestureAction(TouchUMA::kGestureNewTabTap);
  }
}
