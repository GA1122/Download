void Tab::ShowContextMenuForViewImpl(views::View* source,
                                     const gfx::Point& point,
                                     ui::MenuSourceType source_type) {
  if (!closing_)
    controller_->ShowContextMenuForTab(this, point, source_type);
}
