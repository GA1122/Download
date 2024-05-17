MidiManagerUsb::MidiManagerUsb(scoped_ptr<UsbMidiDevice::Factory> factory)
    : device_factory_(factory.Pass()) {
}
