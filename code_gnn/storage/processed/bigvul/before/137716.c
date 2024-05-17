void PrintViewManagerBase::SendPrintingEnabled(bool enabled,
                                               content::RenderFrameHost* rfh) {
  rfh->Send(new PrintMsg_SetPrintingEnabled(rfh->GetRoutingID(), enabled));
}
