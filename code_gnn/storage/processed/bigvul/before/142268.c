void ChromePasswordManagerClient::BindCredentialManager(
    blink::mojom::CredentialManagerRequest request,
    content::RenderFrameHost* render_frame_host) {
  if (render_frame_host->GetParent())
    return;

  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(render_frame_host);
  DCHECK(web_contents);

  if (web_contents->GetMainFrame() != render_frame_host)
    return;

  ChromePasswordManagerClient* instance =
      ChromePasswordManagerClient::FromWebContents(web_contents);

  if (!instance)
    return;

  instance->content_credential_manager_.BindRequest(std::move(request));
}
