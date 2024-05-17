  void DragFromCenterBy(aura::Window* window, int dx, int dy) {
    ui::test::EventGenerator generator(Shell::GetPrimaryRootWindow(), window);
    generator.DragMouseBy(dx, dy);
  }
