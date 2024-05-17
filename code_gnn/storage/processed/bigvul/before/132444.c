void ConvertConfigDescriptor(const UsbConfigDescriptor& input,
                             ConfigDescriptor* output) {
  output->configuration_value = input.configuration_value;
  output->self_powered = input.self_powered;
  output->remote_wakeup = input.remote_wakeup;
  output->max_power = input.maximum_power;
  for (const UsbInterfaceDescriptor& input_interface : input.interfaces) {
    linked_ptr<InterfaceDescriptor> interface(new InterfaceDescriptor);
    ConvertInterfaceDescriptor(input_interface, interface.get());
    output->interfaces.push_back(interface);
  }
  output->extra_data.assign(input.extra_data.begin(), input.extra_data.end());
}
