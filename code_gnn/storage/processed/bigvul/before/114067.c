void TaskManagerView::ShowContextMenuForView(views::View* source,
                                             const gfx::Point& point) {
  UpdateStatsCounters();
  scoped_ptr<views::Menu> menu(views::Menu::Create(
      this, views::Menu::TOPLEFT, source->GetWidget()->GetNativeView()));
  for (std::vector<ui::TableColumn>::iterator i =
       columns_.begin(); i != columns_.end(); ++i) {
    menu->AppendMenuItem(i->id, l10n_util::GetStringUTF16(i->id),
        views::Menu::CHECKBOX);
  }
  menu->RunMenuAt(point.x(), point.y());
}
