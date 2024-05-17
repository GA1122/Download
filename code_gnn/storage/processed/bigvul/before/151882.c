blink::mojom::FileChooserPtr RenderFrameHostImpl::BindFileChooserForTesting() {
  blink::mojom::FileChooserPtr chooser;
  FileChooserImpl::Create(this, mojo::MakeRequest(&chooser));
  return chooser;
}
