void NaClProcessHost::OnResourcesReady() {
  NaClBrowser* nacl_browser = NaClBrowser::GetInstance();
  if (!nacl_browser->IsReady() || !SendStart()) {
    DLOG(ERROR) << "Cannot launch NaCl process";
    delete this;
   }
 }
