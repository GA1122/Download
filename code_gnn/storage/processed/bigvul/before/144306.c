  KeyboardDrivenOobeKeyHandler() {
    ash::Shell::Get()->AddPreTargetHandler(this);
  }
