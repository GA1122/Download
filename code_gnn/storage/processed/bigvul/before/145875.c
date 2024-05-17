  std::unique_ptr<aura::Window> CreateUnsnappableWindow(
      const gfx::Rect& bounds = gfx::Rect()) {
    std::unique_ptr<aura::Window> window;
    if (bounds.IsEmpty())
      window = CreateTestWindow();
    else
      window = base::WrapUnique<aura::Window>(CreateWindow(bounds));

    window->SetProperty(aura::client::kResizeBehaviorKey,
                        ui::mojom::kResizeBehaviorNone);
    return window;
  }
