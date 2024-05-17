bool FakeCentral::IsPresent() const {
  switch (state_) {
    case mojom::CentralState::ABSENT:
      return false;
    case mojom::CentralState::POWERED_OFF:
    case mojom::CentralState::POWERED_ON:
      return true;
  }
  NOTREACHED();
  return false;
}
