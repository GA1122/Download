bool ChromeContentBrowserClient::BindAssociatedInterfaceRequestFromFrame(
    content::RenderFrameHost* render_frame_host,
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle* handle) {
  if (interface_name == autofill::mojom::AutofillDriver::Name_) {
    autofill::ContentAutofillDriverFactory::BindAutofillDriver(
        autofill::mojom::AutofillDriverAssociatedRequest(std::move(*handle)),
        render_frame_host);
    return true;
  }

  return false;
}
