void AuthenticatorWelcomeSheetModel::OnAccept() {
  dialog_model()
      ->StartGuidedFlowForMostLikelyTransportOrShowTransportSelection();
}
