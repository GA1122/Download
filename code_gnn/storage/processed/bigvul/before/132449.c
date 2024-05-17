void ConvertInterfaceDescriptor(const UsbInterfaceDescriptor& input,
                                InterfaceDescriptor* output) {
  output->interface_number = input.interface_number;
  output->alternate_setting = input.alternate_setting;
  output->interface_class = input.interface_class;
  output->interface_subclass = input.interface_subclass;
  output->interface_protocol = input.interface_protocol;
  for (const UsbEndpointDescriptor& input_endpoint : input.endpoints) {
    linked_ptr<EndpointDescriptor> endpoint(new EndpointDescriptor);
    ConvertEndpointDescriptor(input_endpoint, endpoint.get());
    output->endpoints.push_back(endpoint);
  }
  output->extra_data.assign(input.extra_data.begin(), input.extra_data.end());
}
