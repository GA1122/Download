ui::MenuModel* ShellDelegateImpl::CreateContextMenu(WmShelf* wm_shelf,
                                                    const ShelfItem* item) {
  return new ContextMenu(wm_shelf);
}
