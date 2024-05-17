void ChromeContentBrowserClient::LogInitiatorSchemeBypassingDocumentBlocking(
    const url::Origin& initiator_origin,
    int render_process_id,
    content::ResourceType resource_type) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  ChromeContentBrowserClientExtensionsPart::
      LogInitiatorSchemeBypassingDocumentBlocking(
          initiator_origin, render_process_id, resource_type);
#endif
}
