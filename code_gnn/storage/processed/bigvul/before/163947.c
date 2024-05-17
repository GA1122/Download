 base::string16 PaymentHandlerWebFlowViewController::GetSheetTitle() {
   return GetPaymentHandlerDialogTitle(web_contents(), https_prefix_);
}
