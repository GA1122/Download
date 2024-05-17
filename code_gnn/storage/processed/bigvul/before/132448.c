void ConvertEndpointDescriptor(const UsbEndpointDescriptor& input,
                               EndpointDescriptor* output) {
  output->address = input.address;
  output->type = ConvertTransferTypeToApi(input.transfer_type);
  output->direction = ConvertDirectionToApi(input.direction);
  output->maximum_packet_size = input.maximum_packet_size;
  output->synchronization =
      ConvertSynchronizationTypeToApi(input.synchronization_type);
  output->usage = ConvertUsageTypeToApi(input.usage_type);
  output->polling_interval.reset(new int(input.polling_interval));
  output->extra_data.assign(input.extra_data.begin(), input.extra_data.end());
}
