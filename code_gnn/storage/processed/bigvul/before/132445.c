void ConvertDeviceFilter(const usb::DeviceFilter& input,
                         UsbDeviceFilter* output) {
  if (input.vendor_id) {
    output->SetVendorId(*input.vendor_id);
  }
  if (input.product_id) {
    output->SetProductId(*input.product_id);
  }
  if (input.interface_class) {
    output->SetInterfaceClass(*input.interface_class);
  }
  if (input.interface_subclass) {
    output->SetInterfaceSubclass(*input.interface_subclass);
  }
  if (input.interface_protocol) {
    output->SetInterfaceProtocol(*input.interface_protocol);
  }
}
