void BrowserActionView::PaintChildren(gfx::Canvas* canvas) {
  View::PaintChildren(canvas);
  ExtensionAction* action = button()->browser_action();
  int tab_id = panel_->GetCurrentTabId();
  if (tab_id < 0)
    return;

  action->PaintBadge(canvas, gfx::Rect(width(), height()), tab_id);
}
