  explicit ScrollableShelfFocusSearch(
      ScrollableShelfView* scrollable_shelf_view)
      : FocusSearch( nullptr,
                     true,
                     true),
        scrollable_shelf_view_(scrollable_shelf_view) {}
