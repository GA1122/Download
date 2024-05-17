 PepperFlashRendererHost::PepperFlashRendererHost(
     content::RendererPpapiHost* host,
     PP_Instance instance,
    PP_Resource resource)
    : ResourceHost(host->GetPpapiHost(), instance, resource),
      host_(host),
      weak_factory_(this) {
}
