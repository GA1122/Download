bool OSExchangeDataProviderAuraX11::Dispatch(const base::NativeEvent& event) {
  XEvent* xev = event;
  switch (xev->type) {
    case SelectionRequest:
      selection_owner_.OnSelectionRequest(xev->xselectionrequest);
      break;
    default:
      NOTIMPLEMENTED();
  }

  return true;
}
