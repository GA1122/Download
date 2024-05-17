  ConvertToScreenEventHandler() : could_convert_to_screen_(true) {
    aura::Env::GetInstance()->AddPreTargetHandler(this);
  }
