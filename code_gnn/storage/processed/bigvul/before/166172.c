  mojom::MediaStreamDeviceObserverPtr CreateInterfacePtrAndBind() {
    mojom::MediaStreamDeviceObserverPtr observer;
    binding_.Bind(mojo::MakeRequest(&observer));
    return observer;
  }
