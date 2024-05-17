  explicit PdfServiceTestClient(service_manager::test::ServiceTest* test)
      : service_manager::test::ServiceTestClient(test) {
    registry_.AddInterface<service_manager::mojom::ServiceFactory>(
        base::Bind(&PdfServiceTestClient::Create, base::Unretained(this)));
  }
