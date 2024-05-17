  static void Create(RenderFrameHostImpl* render_frame_host,
                     blink::mojom::FileChooserRequest request) {
    mojo::MakeStrongBinding(
        std::make_unique<FileChooserImpl>(render_frame_host),
        std::move(request));
  }
