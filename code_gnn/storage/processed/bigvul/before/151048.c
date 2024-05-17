void DevToolsWindow::ActivateContents(WebContents* contents) {
  if (is_docked_) {
    WebContents* inspected_tab = GetInspectedWebContents();
    if (inspected_tab)
      inspected_tab->GetDelegate()->ActivateContents(inspected_tab);
  } else if (browser_) {
    browser_->window()->Activate();
  }
}
