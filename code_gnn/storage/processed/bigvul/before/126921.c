  void RunMenuAt(const gfx::Point& point) {
    if (menu_runner_->RunMenuAt(
            tab_->GetWidget(), NULL, gfx::Rect(point, gfx::Size()),
            views::MenuItemView::TOPLEFT, views::MenuRunner::HAS_MNEMONICS |
            views::MenuRunner::CONTEXT_MENU) ==
        views::MenuRunner::MENU_DELETED)
      return;
  }
