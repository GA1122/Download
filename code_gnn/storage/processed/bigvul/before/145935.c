  void CheckWindowAndItemPlacement(aura::Window* first, aura::Window* second) {
    Shelf* shelf = GetPrimaryShelf();
    const gfx::Rect first_item_bounds =
        shelf->GetScreenBoundsOfItemIconForWindow(first);
    const gfx::Rect second_item_bounds =
        shelf->GetScreenBoundsOfItemIconForWindow(second);
    if (!base::i18n::IsRTL()) {
      EXPECT_TRUE((first->bounds().x() < second->bounds().x()) ||
                  (first->bounds().y() < second->bounds().y()));
      EXPECT_TRUE((first_item_bounds.x() < second_item_bounds.x()) ||
                  (first_item_bounds.y() < second_item_bounds.y()));
    } else {
      EXPECT_TRUE((first->bounds().x() > second->bounds().x()) ||
                  (first->bounds().y() < second->bounds().y()));
      EXPECT_TRUE((first_item_bounds.x() > second_item_bounds.x()) ||
                  (first_item_bounds.y() < second_item_bounds.y()));
    }
  }
