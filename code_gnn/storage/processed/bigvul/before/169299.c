RenderFrameMetadataProvider* RenderFrameMetadataProviderFromWebContents(
    WebContents* web_contents) {
  DCHECK(web_contents);
  DCHECK(web_contents->GetRenderViewHost());
  DCHECK(
      RenderWidgetHostImpl::From(web_contents->GetRenderViewHost()->GetWidget())
          ->render_frame_metadata_provider());
  return RenderWidgetHostImpl::From(
             web_contents->GetRenderViewHost()->GetWidget())
      ->render_frame_metadata_provider();
}
