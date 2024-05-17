bool PrintViewManagerBase::PrintNow(content::RenderFrameHost* rfh) {
  DisconnectFromCurrentPrintJob();

  SetPrintingRFH(rfh);
  int32_t id = rfh->GetRoutingID();
  return PrintNowInternal(rfh, base::MakeUnique<PrintMsg_PrintPages>(id));
}
