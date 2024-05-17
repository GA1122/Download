void LocalDOMWindow::SetModulator(Modulator* modulator) {
  DCHECK(!modulator_);
  modulator_ = modulator;
}
