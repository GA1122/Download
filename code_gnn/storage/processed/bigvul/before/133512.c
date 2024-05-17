void SetSecondaryDisplayLayout(DisplayLayout::Position position) {
  DisplayLayout layout =
      Shell::GetInstance()->display_manager()->GetCurrentDisplayLayout();
  layout.position = position;
  Shell::GetInstance()->display_manager()->
      SetLayoutForCurrentDisplays(layout);
}
