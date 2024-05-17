void ExtensionFunctionDispatcher::Dispatch(
    const ExtensionHostMsg_Request_Params& params,
    RenderViewHost* render_view_host) {
  ExtensionService* service = profile()->GetExtensionService();
  if (!service)
    return;

  if (!service->ExtensionBindingsAllowed(params.source_url)) {
    LOG(ERROR) << "Extension bindings not allowed for URL: "
               << params.source_url.spec();
    SendAccessDenied(render_view_host, render_view_host->routing_id(),
                     params.request_id);
    return;
  }

  const Extension* extension = service->GetExtensionByURL(params.source_url);
  if (!extension)
    extension = service->GetExtensionByWebExtent(params.source_url);

  scoped_refptr<ExtensionFunction> function(CreateExtensionFunction(
      params, extension, profile_->GetRuntimeId(),
      render_view_host->process()->id(),
      render_view_host, render_view_host->routing_id()));
  if (!function)
    return;

  UIThreadExtensionFunction* function_ui =
      function->AsUIThreadExtensionFunction();
  if (!function_ui) {
    NOTREACHED();
    return;
  }
  function_ui->SetRenderViewHost(render_view_host);
  function_ui->set_dispatcher(AsWeakPtr());
  function_ui->set_profile(profile_);
  function->set_include_incognito(service->CanCrossIncognito(extension));

  ExtensionsQuotaService* quota = service->quota_service();
  if (quota->Assess(extension->id(), function, &params.arguments,
                    base::TimeTicks::Now())) {
    ExternalProtocolHandler::PermitLaunchUrl();

    function->Run();
  } else {
    render_view_host->Send(new ExtensionMsg_Response(
        render_view_host->routing_id(), function->request_id(), false,
        std::string(), QuotaLimitHeuristic::kGenericOverQuotaError));
  }
}
