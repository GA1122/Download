void PaletteTray::AddToolsToView(views::View* host) {
  std::vector<PaletteToolView> views = palette_tool_manager_->CreateViews();

  if (!views.size())
    return;

  PaletteGroup group = views[0].group;
  for (const PaletteToolView& view : views) {
    if (group != view.group) {
      group = view.group;
      host->AddChildView(CreateSeparator(views::Separator::HORIZONTAL));
    }

    host->AddChildView(view.view);
  }
}
