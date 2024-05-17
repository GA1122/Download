bool PrintingContext::AllocateContext(const std::wstring& printer_name,
                                      const DEVMODE* dev_mode) {
  context_ = CreateDC(L"WINSPOOL", printer_name.c_str(), NULL, dev_mode);
  DCHECK(context_);
  return context_ != NULL;
}
