  void PassLastDocumentInterfaceBrokerRequest(
      blink::mojom::DocumentInterfaceBrokerRequest
          document_interface_broker_request) {
    last_document_interface_broker_request_ =
        std::move(document_interface_broker_request);
  }
