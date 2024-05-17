  void ProcessScrollInternal(ui::EventType type,
                             const gfx::Vector2dF& delta,
                             bool bounds_check) {
    if (GetShelfLayoutManager()->visibility_state() == SHELF_HIDDEN)
      return;

    if (type == ui::ET_GESTURE_SCROLL_BEGIN) {
      scroll_ = gfx::Vector2dF();
      was_visible_on_drag_start_ = GetShelfLayoutManager()->IsVisible();
      return;
    }

    if (type == ui::ET_GESTURE_SCROLL_END)
      return;

    if (type == ui::ET_GESTURE_SCROLL_UPDATE)
      scroll_.Add(delta);

    Shelf* shelf = AshTestBase::GetPrimaryShelf();
    gfx::Rect shelf_bounds = GetShelfWidget()->GetWindowBoundsInScreen();

    float scroll_delta =
        GetShelfLayoutManager()->PrimaryAxisValue(scroll_.y(), scroll_.x());
    bool increasing_drag =
        GetShelfLayoutManager()->SelectValueForShelfAlignment(
            scroll_delta<0, scroll_delta> 0, scroll_delta < 0);
    const int shelf_size = GetShelfLayoutManager()->PrimaryAxisValue(
        shelf_bounds.height(), shelf_bounds.width());
    if (was_visible_on_drag_start_) {
      if (increasing_drag) {
        const int bounds_delta =
            GetShelfLayoutManager()->SelectValueForShelfAlignment(
                visible_shelf_bounds_.y() - shelf_bounds.y(),
                shelf_bounds.x() - visible_shelf_bounds_.x(),
                visible_shelf_bounds_.x() - shelf_bounds.x());
        EXPECT_GE(bounds_delta, 0);
        EXPECT_LE(bounds_delta, std::abs(scroll_delta));
      } else {
        if (SHELF_ALIGNMENT_BOTTOM == shelf->alignment())
          EXPECT_LE(visible_shelf_bounds_.y(), shelf_bounds.y());
        else if (SHELF_ALIGNMENT_LEFT == shelf->alignment())
          EXPECT_LE(shelf_bounds.x(), visible_shelf_bounds_.x());
        else if (SHELF_ALIGNMENT_RIGHT == shelf->alignment())
          EXPECT_LE(visible_shelf_bounds_.x(), shelf_bounds.x());
      }
    } else {
      if (increasing_drag && bounds_check) {
        constexpr float kEpsilon = 1.f;
        if (std::abs(scroll_delta) < shelf_size) {
          if (SHELF_ALIGNMENT_BOTTOM == shelf->alignment()) {
            EXPECT_NEAR(shelf_bounds.y(),
                        auto_hidden_shelf_bounds_.y() +
                            kHiddenShelfInScreenPortion -
                            std::abs(scroll_delta),
                        kEpsilon);
          } else if (SHELF_ALIGNMENT_LEFT == shelf->alignment()) {
            EXPECT_NEAR(shelf_bounds.x(),
                        auto_hidden_shelf_bounds_.x() -
                            kHiddenShelfInScreenPortion +
                            std::abs(scroll_delta),
                        kEpsilon);
          } else if (SHELF_ALIGNMENT_RIGHT == shelf->alignment()) {
            EXPECT_NEAR(shelf_bounds.x(),
                        auto_hidden_shelf_bounds_.x() +
                            kHiddenShelfInScreenPortion -
                            std::abs(scroll_delta),
                        kEpsilon);
          }
        } else {
          if (SHELF_ALIGNMENT_BOTTOM == shelf->alignment()) {
            EXPECT_GT(shelf_bounds.y(), auto_hidden_shelf_bounds_.y() +
                                            kHiddenShelfInScreenPortion -
                                            std::abs(scroll_delta));
          } else if (SHELF_ALIGNMENT_LEFT == shelf->alignment()) {
            EXPECT_LT(shelf_bounds.x(), auto_hidden_shelf_bounds_.x() -
                                            kHiddenShelfInScreenPortion +
                                            std::abs(scroll_delta));
          } else if (SHELF_ALIGNMENT_RIGHT == shelf->alignment()) {
            EXPECT_GT(shelf_bounds.x(), auto_hidden_shelf_bounds_.x() +
                                            kHiddenShelfInScreenPortion -
                                            std::abs(scroll_delta));
          }
        }
      }
    }
  }
