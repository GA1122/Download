  void UpdateDisplay(const std::string& display_specs) {
    display::test::DisplayManagerTestApi(ash::Shell::Get()->display_manager())
        .UpdateDisplay(display_specs);
  }
