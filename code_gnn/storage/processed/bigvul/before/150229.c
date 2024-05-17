  aura::Window* CreateWindowInWatchedContainer(const InitParams& params) {
    aura::test::TestWindowDelegate* delegate = NULL;
    if (!params.can_maximize) {
      delegate = aura::test::TestWindowDelegate::CreateSelfDestroyingDelegate();
      delegate->set_window_component(HTCAPTION);
      if (!params.max_size.IsEmpty())
        delegate->set_maximum_size(params.max_size);
    }
    aura::Window* window = aura::test::CreateTestWindowWithDelegateAndType(
        delegate, params.type, 0, params.bounds, NULL, params.show_on_creation);
    int32_t behavior = aura::client::kResizeBehaviorNone;
    behavior |= params.can_resize ? aura::client::kResizeBehaviorCanResize : 0;
    behavior |=
        params.can_maximize ? aura::client::kResizeBehaviorCanMaximize : 0;
    window->SetProperty(aura::client::kResizeBehaviorKey, behavior);
    aura::Window* container =
        wm::GetSwitchableContainersForRoot(Shell::GetPrimaryRootWindow(),
                                            true)[0];
    container->AddChild(window);
    return window;
  }
