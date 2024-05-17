bool UsbTabHelper::AllowedByFeaturePolicy(
    RenderFrameHost* render_frame_host) const {
  DCHECK(WebContents::FromRenderFrameHost(render_frame_host) == web_contents());
  return render_frame_host->IsFeatureEnabled(
      blink::mojom::FeaturePolicyFeature::kUsb);
}
