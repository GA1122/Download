void ShelfLayoutManagerTest::TestHomeLauncherGestureHandler(
    bool autohide_shelf) {
  TabletModeControllerTestApi().EnterTabletMode();

  Shelf* shelf = GetPrimaryShelf();
  if (autohide_shelf)
    shelf->SetAutoHideBehavior(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS);

  std::unique_ptr<aura::Window> extra_window =
      AshTestBase::CreateTestWindow(gfx::Rect(100, 10, 100, 100));
  std::unique_ptr<aura::Window> window =
      AshTestBase::CreateTestWindow(gfx::Rect(0, 0, 400, 400));
  wm::ActivateWindow(window.get());

  if (autohide_shelf) {
    gfx::Rect display_bounds =
        display::Screen::GetScreen()->GetPrimaryDisplay().bounds();
    const gfx::Point start(display_bounds.bottom_center());
    const gfx::Point end(start + gfx::Vector2d(0, -80));
    const base::TimeDelta kTimeDelta = base::TimeDelta::FromMilliseconds(100);
    const int kNumScrollSteps = 4;
    GetEventGenerator()->GestureScrollSequence(start, end, kTimeDelta,
                                               kNumScrollSteps);
    EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  }

  const gfx::Point shelf_center =
      GetVisibleShelfWidgetBoundsInScreen().CenterPoint();

  auto create_scroll_event = [&shelf_center](ui::EventType type,
                                             float scroll_y) {
    ui::GestureEventDetails details =
        type == ui::ET_GESTURE_SCROLL_END
            ? ui::GestureEventDetails(type)
            : ui::GestureEventDetails(type, 0, scroll_y);
    return ui::GestureEvent(shelf_center.x(), shelf_center.y(), 0,
                            base::TimeTicks(), details);
  };

  HomeLauncherGestureHandler* gesture_handler =
      Shell::Get()->app_list_controller()->home_launcher_gesture_handler();
  ASSERT_TRUE(gesture_handler);
  ASSERT_FALSE(gesture_handler->GetWindow1());

  ShelfLayoutManager* manager = GetShelfLayoutManager();
  manager->ProcessGestureEvent(
      create_scroll_event(ui::ET_GESTURE_SCROLL_BEGIN, -1.f));
  EXPECT_EQ(window.get(), gesture_handler->GetWindow1());
  EXPECT_EQ(SHELF_BACKGROUND_DEFAULT, GetShelfWidget()->GetBackgroundType());
  if (autohide_shelf) {
    EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  }

  manager->ProcessGestureEvent(
      create_scroll_event(ui::ET_GESTURE_SCROLL_UPDATE, -1.f));
  EXPECT_EQ(window.get(), gesture_handler->GetWindow1());
  EXPECT_EQ(SHELF_BACKGROUND_DEFAULT, GetShelfWidget()->GetBackgroundType());

  manager->ProcessGestureEvent(
      create_scroll_event(ui::ET_GESTURE_SCROLL_UPDATE, 1.f));
  EXPECT_EQ(window.get(), gesture_handler->GetWindow1());
  EXPECT_EQ(SHELF_BACKGROUND_DEFAULT, GetShelfWidget()->GetBackgroundType());
  if (autohide_shelf)
    EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());

  manager->ProcessGestureEvent(
      create_scroll_event(ui::ET_GESTURE_SCROLL_END, 1.f));
  ASSERT_FALSE(gesture_handler->GetWindow1());
  if (autohide_shelf)
    EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());

  manager->ProcessGestureEvent(
      create_scroll_event(ui::ET_GESTURE_SCROLL_BEGIN, 1.f));
  EXPECT_FALSE(gesture_handler->GetWindow1());
  manager->ProcessGestureEvent(
      create_scroll_event(ui::ET_GESTURE_SCROLL_UPDATE, -1.f));
  EXPECT_FALSE(gesture_handler->GetWindow1());
}
