  const views::Widget* GetWindowCycleListWidget() const {
    return Shell::Get()
        ->window_cycle_controller()
        ->window_cycle_list()
        ->widget();
  }
