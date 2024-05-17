void ShelfLayoutManagerTest::RunGestureDragTests(
    const gfx::Point& edge_to_hide,
    const gfx::Point& edge_to_show) {
  ui::test::EventGenerator* generator = GetEventGenerator();
  display::Display display = display::Screen::GetScreen()->GetPrimaryDisplay();
  generator->MoveMouseTo(display.bounds().CenterPoint());

  Shelf* shelf = GetPrimaryShelf();
  shelf->SetAutoHideBehavior(SHELF_AUTO_HIDE_BEHAVIOR_NEVER);

  views::Widget* widget = CreateTestWidget();
  widget->Maximize();

  const base::TimeDelta kTimeDelta = base::TimeDelta::FromMilliseconds(100);

  aura::Window* window = widget->GetNativeWindow();
  ShelfLayoutManager* layout_manager = GetShelfLayoutManager();
  layout_manager->LayoutShelf();

  gfx::Rect shelf_shown = GetShelfWidget()->GetWindowBoundsInScreen();
  gfx::Rect window_bounds_with_shelf = window->bounds();
  EXPECT_EQ(SHELF_VISIBLE, shelf->GetVisibilityState());

  shelf->SetAutoHideBehavior(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS);
  layout_manager->LayoutShelf();
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());

  gfx::Rect window_bounds_with_noshelf = window->bounds();
  gfx::Rect shelf_hidden = GetShelfWidget()->GetWindowBoundsInScreen();

  shelf->SetAutoHideBehavior(SHELF_AUTO_HIDE_BEHAVIOR_NEVER);
  layout_manager->LayoutShelf();

  const int kNumScrollSteps = 4;
  ShelfDragCallback handler(shelf_hidden, shelf_shown);

  {
    SCOPED_TRACE("SWIPE_DOWN_ALWAYS_SHOWN");
    generator->GestureScrollSequenceWithCallback(
        edge_to_hide, edge_to_show, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_VISIBLE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_NEVER, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_shelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  widget->SetFullscreen(true);
  wm::WindowState* window_state = wm::GetWindowState(window);
  window_state->SetHideShelfWhenFullscreen(false);
  window->SetProperty(kImmersiveIsActive, true);
  layout_manager->UpdateVisibilityState();
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_NEVER, shelf->auto_hide_behavior());

  generator->GestureScrollSequence(edge_to_hide, edge_to_show, kTimeDelta,
                                   kNumScrollSteps);
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_NEVER, shelf->auto_hide_behavior());

  generator->GestureScrollSequence(edge_to_show, edge_to_hide, kTimeDelta,
                                   kNumScrollSteps);
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_NEVER, shelf->auto_hide_behavior());

  widget->SetFullscreen(false);
  EXPECT_EQ(SHELF_VISIBLE, shelf->GetVisibilityState());

  window_state->Minimize();
  EXPECT_EQ(SHELF_VISIBLE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_NEVER, shelf->auto_hide_behavior());

  window_state->Maximize();
  shelf->SetAutoHideBehavior(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS);
  layout_manager->LayoutShelf();
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());

  {
    SCOPED_TRACE("SWIPE_UP_AUTO_HIDE_SHOW");
    generator->GestureScrollSequenceWithCallback(
        edge_to_hide, edge_to_show, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  gfx::Point new_point(edge_to_show);
  gfx::Vector2d diff = edge_to_hide - edge_to_show;
  new_point.Offset(diff.x() * 3 / 10, diff.y() * 3 / 10);
   
  generator->GestureScrollSequence(edge_to_show, new_point, kTimeDelta, 5);
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  {
    SCOPED_TRACE("SWIPE_DOWN_AUTO_HIDE_1");
    generator->GestureScrollSequenceWithCallback(
        edge_to_show, edge_to_hide, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_hidden.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  gfx::Point extended_start = edge_to_show;
  if (shelf->IsHorizontalAlignment())
    extended_start.set_y(GetShelfWidget()->GetWindowBoundsInScreen().y() - 1);
  else if (SHELF_ALIGNMENT_LEFT == shelf->alignment())
    extended_start.set_x(GetShelfWidget()->GetWindowBoundsInScreen().right() +
                         1);
  else if (SHELF_ALIGNMENT_RIGHT == shelf->alignment())
    extended_start.set_x(GetShelfWidget()->GetWindowBoundsInScreen().x() - 1);
  {
    SCOPED_TRACE("SWIPE_UP_EXTENDED_HIT");
    generator->GestureScrollSequenceWithCallback(
        extended_start, edge_to_show, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }

  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  {
    SCOPED_TRACE("SWIPE_DOWN_AUTO_HIDE_2");
    generator->GestureScrollSequenceWithCallback(
        edge_to_show, edge_to_hide, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_hidden.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  gfx::Point outside_start =
      GetShelfWidget()->GetWindowBoundsInScreen().top_center();
  outside_start.set_y(outside_start.y() - 50);
  gfx::Vector2d delta = edge_to_hide - edge_to_show;
  generator->GestureScrollSequence(outside_start, outside_start + delta,
                                   kTimeDelta, kNumScrollSteps);
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_hidden.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());
  gfx::Point below_start = edge_to_hide;
  if (shelf->IsHorizontalAlignment())
    below_start.set_y(GetShelfWidget()->GetWindowBoundsInScreen().bottom() - 1);
  else if (SHELF_ALIGNMENT_LEFT == shelf->alignment())
    below_start.set_x(GetShelfWidget()->GetWindowBoundsInScreen().x());
  else if (SHELF_ALIGNMENT_RIGHT == shelf->alignment())
    below_start.set_x(GetShelfWidget()->GetWindowBoundsInScreen().right() - 1);
  generator->GestureScrollSequence(below_start, edge_to_show, kTimeDelta,
                                   kNumScrollSteps);
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  {
    SCOPED_TRACE("SWIPE_DOWN_AUTO_HIDE_3");
    generator->GestureScrollSequenceWithCallback(
        edge_to_show, edge_to_hide, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(window_bounds_with_noshelf.ToString(), window->bounds().ToString());
  EXPECT_EQ(shelf_hidden.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  widget->SetFullscreen(true);
  wm::GetWindowState(window)->SetHideShelfWhenFullscreen(false);
  layout_manager->UpdateVisibilityState();

  gfx::Rect window_bounds_fullscreen = window->bounds();
  EXPECT_TRUE(widget->IsFullscreen());

  EXPECT_EQ(window_bounds_with_noshelf.ToString(),
            window_bounds_fullscreen.ToString());

  {
    SCOPED_TRACE("SWIPE_UP_AUTO_HIDE_1");
    generator->GestureScrollSequenceWithCallback(
        below_start, edge_to_show, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScrollNoBoundsCheck,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());
  EXPECT_EQ(window_bounds_fullscreen.ToString(), window->bounds().ToString());

  {
    SCOPED_TRACE("SWIPE_DOWN_AUTO_HIDE_4");
    generator->GestureScrollSequenceWithCallback(
        edge_to_show, edge_to_hide, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_HIDDEN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(shelf_hidden.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());
  EXPECT_EQ(window_bounds_fullscreen.ToString(), window->bounds().ToString());

  wm::GetWindowState(window)->SetHideShelfWhenFullscreen(true);

  layout_manager->UpdateVisibilityState();
  EXPECT_EQ(SHELF_HIDDEN, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());

  {
    SCOPED_TRACE("SWIPE_UP_NO_CHANGE");
    generator->GestureScrollSequenceWithCallback(
        below_start, edge_to_show, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
    EXPECT_EQ(SHELF_HIDDEN, shelf->GetVisibilityState());
    EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
    EXPECT_EQ(window_bounds_fullscreen.ToString(), window->bounds().ToString());
  }

  widget->Minimize();
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(layout_manager->HasVisibleWindow());

  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());

  {
    SCOPED_TRACE("SWIPE_DOWN_AUTO_HIDE_5");
    generator->GestureScrollSequenceWithCallback(
        edge_to_show, edge_to_hide, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  widget->Restore();
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(layout_manager->HasVisibleWindow());

  {
    SCOPED_TRACE("SWIPE_UP_AUTO_HIDE_2");
    generator->GestureScrollSequenceWithCallback(
        below_start, edge_to_show, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScrollNoBoundsCheck,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  widget->Close();
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(layout_manager->HasVisibleWindow());

  {
    SCOPED_TRACE("SWIPE_DOWN_AUTO_HIDE_6");
    generator->GestureScrollSequenceWithCallback(
        edge_to_show, edge_to_hide, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS, shelf->auto_hide_behavior());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());

  {
    SCOPED_TRACE("SWIPE_UP_AUTO_HIDE_4");
    generator->GestureScrollSequenceWithCallback(
        edge_to_hide, edge_to_show, kTimeDelta, kNumScrollSteps,
        base::BindRepeating(&ShelfDragCallback::ProcessScroll,
                            base::Unretained(&handler)));
  }
  EXPECT_EQ(SHELF_AUTO_HIDE, shelf->GetVisibilityState());
  EXPECT_EQ(SHELF_AUTO_HIDE_SHOWN, shelf->GetAutoHideState());
  EXPECT_EQ(shelf_shown.ToString(),
            GetShelfWidget()->GetWindowBoundsInScreen().ToString());
}
