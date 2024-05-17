MediaInterfaceProxy::MediaInterfaceProxy(
    RenderFrameHost* render_frame_host,
    media::mojom::InterfaceFactoryRequest request,
    const base::Closure& error_handler)
    : render_frame_host_(render_frame_host),
      binding_(this, std::move(request)) {
  DVLOG(1) << __FUNCTION__;
  DCHECK(render_frame_host_);
  DCHECK(!error_handler.is_null());

  binding_.set_connection_error_handler(error_handler);

}
