void BrowserTabStripController::ShowContextMenuForTab(BaseTab* tab,
                                                      const gfx::Point& p) {
  context_menu_contents_.reset(new TabContextMenuContents(tab, this));
  context_menu_contents_->RunMenuAt(p);
}
