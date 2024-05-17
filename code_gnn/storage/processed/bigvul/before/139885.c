void MidiManagerUsb::OnEnumerateDevicesDone(bool result,
                                            UsbMidiDevice::Devices* devices) {
  if (!result) {
    initialize_callback_.Run(MIDI_INITIALIZATION_ERROR);
    return;
  }
  devices->swap(devices_);
  for (size_t i = 0; i < devices_.size(); ++i) {
    UsbMidiDescriptorParser parser;
    std::vector<uint8> descriptor = devices_[i]->GetDescriptor();
    const uint8* data = descriptor.size() > 0 ? &descriptor[0] : NULL;
    std::vector<UsbMidiJack> jacks;
    bool parse_result = parser.Parse(devices_[i],
                                     data,
                                     descriptor.size(),
                                     &jacks);
    if (!parse_result) {
      initialize_callback_.Run(MIDI_INITIALIZATION_ERROR);
      return;
    }
    std::vector<UsbMidiJack> input_jacks;
    for (size_t j = 0; j < jacks.size(); ++j) {
      if (jacks[j].direction() == UsbMidiJack::DIRECTION_OUT) {
        output_streams_.push_back(new UsbMidiOutputStream(jacks[j]));
        MidiPortInfo port;
        port.id = base::StringPrintf("port-%ld-%ld",
                                     static_cast<long>(i),
                                     static_cast<long>(j));
        AddOutputPort(port);
      } else {
        DCHECK_EQ(jacks[j].direction(), UsbMidiJack::DIRECTION_IN);
        input_jacks.push_back(jacks[j]);
        MidiPortInfo port;
        port.id = base::StringPrintf("port-%ld-%ld",
                                     static_cast<long>(i),
                                     static_cast<long>(j));
        AddInputPort(port);
      }
    }
    input_stream_.reset(new UsbMidiInputStream(input_jacks, this));
  }
  initialize_callback_.Run(MIDI_OK);
}
