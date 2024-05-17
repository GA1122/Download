bool Textfield::CanHandleAccelerators() const {
  return GetRenderText()->focused() && View::CanHandleAccelerators();
}
