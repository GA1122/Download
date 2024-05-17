void PaymentHandlerWebFlowViewController::AbortPayment() {
  if (web_contents())
    web_contents()->Close();

  dialog()->ShowErrorMessage();
}
