PP_Resource PPB_URLLoader_Impl::GetResponseInfo() {
  if (!response_info_)
    return 0;
  return response_info_->GetReference();
}
