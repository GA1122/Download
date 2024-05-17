void RenderFrameHostImpl::BindDocumentInterfaceBrokerRequest(
    blink::mojom::DocumentInterfaceBrokerRequest content_request,
    blink::mojom::DocumentInterfaceBrokerRequest blink_request) {
  DCHECK(!document_interface_broker_content_binding_.is_bound());
  DCHECK(content_request.is_pending());
  document_interface_broker_content_binding_.Bind(std::move(content_request));
  DCHECK(!document_interface_broker_blink_binding_.is_bound());
  DCHECK(blink_request.is_pending());
  document_interface_broker_blink_binding_.Bind(std::move(blink_request));
}
