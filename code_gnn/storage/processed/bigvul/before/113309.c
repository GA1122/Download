NativePanelTestingWin::NativePanelTestingWin(
    PanelBrowserView* panel_browser_view) :
    panel_browser_view_(panel_browser_view) {
  PanelBrowserFrameView* frame_view = panel_browser_view_->GetFrameView();
  frame_view->title_label_->SetAutoColorReadabilityEnabled(false);
}
