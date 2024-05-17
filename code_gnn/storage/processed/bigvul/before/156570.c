  ScopedInterfaceProviderRequestReplacer(
      WebContents* web_contents,
      service_manager::mojom::InterfaceProviderRequest
          interface_provider_request_override)
      : DidCommitProvisionalLoadInterceptor(web_contents),
        interface_provider_request_override_(
            std::move(interface_provider_request_override)) {}
