void TabStrip::ButtonPressed(views::Button* sender, const ui::Event& event) {
  if (sender == newtab_button_) {
    content::RecordAction(UserMetricsAction("NewTab_Button"));
    UMA_HISTOGRAM_ENUMERATION("Tab.NewTab", TabStripModel::NEW_TAB_BUTTON,
                              TabStripModel::NEW_TAB_ENUM_COUNT);
    if (event.IsMouseEvent()) {
      const ui::MouseEvent& mouse = static_cast<const ui::MouseEvent&>(event);
      if (mouse.IsOnlyMiddleMouseButton()) {
        base::string16 clipboard_text = GetClipboardText();
        if (!clipboard_text.empty())
          controller()->CreateNewTabWithLocation(clipboard_text);
        return;
      }
    }

    controller()->CreateNewTab();
    if (event.type() == ui::ET_GESTURE_TAP)
      TouchUMA::RecordGestureAction(TouchUMA::GESTURE_NEWTAB_TAP);
  }
}
