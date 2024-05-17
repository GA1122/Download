void RenderFrameImpl::SetDocumentInterfaceBrokerForTesting(
    blink::mojom::DocumentInterfaceBrokerPtr test_broker) {
  document_interface_broker_.FlushForTesting();
  document_interface_broker_ = std::move(test_broker);
}
