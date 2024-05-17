void BrowserView::RotatePaneFocus(bool forwards) {
  if (GetLocationBarView()->ActivateFirstInactiveBubbleForAccessibility())
    return;

  GetFocusManager()->RotatePaneFocus(
      forwards ?
          views::FocusManager::kForward : views::FocusManager::kBackward,
      views::FocusManager::kWrap);
}
