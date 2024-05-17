void RenderFrameHostImpl::CreateUsbDeviceManager(
    device::mojom::UsbDeviceManagerRequest request) {
  GetContentClient()->browser()->CreateUsbDeviceManager(this,
                                                        std::move(request));
}
