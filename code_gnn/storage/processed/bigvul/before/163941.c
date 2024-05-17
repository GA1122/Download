void PaymentHandlerWebFlowViewController::DidAttachInterstitialPage() {
  log_.Error("Aborting payment handler window \"" + target_.spec() +
             "\" because of navigation to a page with invalid certificate "
             "state or malicious content.");
  AbortPayment();
}
