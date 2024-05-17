void ContextState::InitStateManual(const ContextState*) const {
  UpdatePackParameters();
  UpdateUnpackParameters();
  UpdateWindowRectangles();
}
