  void GenerateEvents(aura::Window* window) {
    ui::test::EventGenerator event_generator(Shell::GetPrimaryRootWindow(),
                                             window);
    event_generator.ClickLeftButton();
    event_generator.ScrollSequence(window->bounds().CenterPoint(),
                                   base::TimeDelta(), 0, 10, 1, 2);
    event_generator.PressTouch();
    event_generator.ReleaseTouch();
    event_generator.GestureTapAt(window->bounds().CenterPoint());
  }
