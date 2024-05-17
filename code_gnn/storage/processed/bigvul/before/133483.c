int32_t PepperFlashRendererHost::OnInvokePrinting(
    ppapi::host::HostMessageContext* host_context) {
  PPB_PDF_Impl::InvokePrintingForInstance(pp_instance());
  return PP_OK;
}
