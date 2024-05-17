void LauncherView::ShowContextMenuForView(views::View* source,
                                          const gfx::Point& point) {
  if (!delegate_)
    return;

  int view_index = view_model_->GetIndexOfView(source);
  if (view_index != -1 &&
      model_->items()[view_index].type == TYPE_APP_LIST) {
    view_index = -1;
  }
#if !defined(OS_MACOSX)
  scoped_ptr<ui::MenuModel> menu_model(
      view_index == -1 ?
          delegate_->CreateContextMenuForLauncher() :
          delegate_->CreateContextMenu(model_->items()[view_index]));
  if (!menu_model.get())
    return;
  AutoReset<LauncherID> reseter(
      &context_menu_id_,
      view_index == -1 ? 0 : model_->items()[view_index].id);
  views::MenuModelAdapter menu_model_adapter(menu_model.get());
  launcher_menu_runner_.reset(
      new views::MenuRunner(menu_model_adapter.CreateMenu()));
  if (launcher_menu_runner_->RunMenuAt(
          source->GetWidget(), NULL, gfx::Rect(point, gfx::Size()),
          views::MenuItemView::TOPLEFT, 0) == views::MenuRunner::MENU_DELETED)
    return;

  Shell::GetInstance()->UpdateShelfVisibility();
#endif
}
