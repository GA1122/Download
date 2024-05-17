  void DragAlongShelfReorder(int dx, int dy) {
    std::unique_ptr<aura::Window> w1(CreatePanelWindow(gfx::Point()));
    std::unique_ptr<aura::Window> w2(CreatePanelWindow(gfx::Point()));
    CheckWindowAndItemPlacement(w2.get(), w1.get());

    DragStart(w1.get());
    DragMove(400 * dx, 400 * dy);
    CheckWindowAndItemPlacement(w1.get(), w2.get());
    DragEnd();
    CheckWindowAndItemPlacement(w1.get(), w2.get());

    DragStart(w1.get());
    DragMove(-400 * dx, -400 * dy);
    CheckWindowAndItemPlacement(w2.get(), w1.get());
    DragEnd();
    CheckWindowAndItemPlacement(w2.get(), w1.get());
  }
