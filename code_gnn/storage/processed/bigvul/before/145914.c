  void ClickShelfItemForWindow(ShelfView* shelf_view, aura::Window* window) {
    ShelfViewTestAPI test_api(shelf_view);
    test_api.SetAnimationDuration(1);
    test_api.RunMessageLoopUntilAnimationsDone();
    ShelfID shelf_id = ShelfID::Deserialize(window->GetProperty(kShelfIDKey));
    DCHECK(!shelf_id.IsNull());
    int index = Shell::Get()->shelf_model()->ItemIndexByID(shelf_id);
    DCHECK_GE(index, 0);
    gfx::Rect bounds = test_api.GetButton(index)->GetBoundsInScreen();

    ui::test::EventGenerator& event_generator = GetEventGenerator();
    event_generator.MoveMouseTo(bounds.CenterPoint());
    event_generator.ClickLeftButton();

    test_api.RunMessageLoopUntilAnimationsDone();
  }
