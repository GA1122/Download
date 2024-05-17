void ChromeContentBrowserClient::CreateWebUsbService(
    content::RenderFrameHost* render_frame_host,
    mojo::InterfaceRequest<blink::mojom::WebUsbService> request) {
  if (!base::FeatureList::IsEnabled(features::kWebUsb))
    return;

#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (render_frame_host->GetSiteInstance()->GetSiteURL().SchemeIs(
          extensions::kExtensionScheme)) {
    return;
  }
#endif

  WebContents* web_contents =
      WebContents::FromRenderFrameHost(render_frame_host);
  if (!web_contents) {
    NOTREACHED();
    return;
  }

  UsbTabHelper* tab_helper =
      UsbTabHelper::GetOrCreateForWebContents(web_contents);
  tab_helper->CreateWebUsbService(render_frame_host, std::move(request));
}
