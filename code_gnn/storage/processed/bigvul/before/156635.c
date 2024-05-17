  void Intercept(
      mojo::StrongAssociatedBindingPtr<blink::mojom::BlobURLStore> binding) {
    url_store_ = binding->SwapImplForTesting(this);
  }
