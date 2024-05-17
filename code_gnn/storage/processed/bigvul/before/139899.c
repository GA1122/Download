  MidiManagerUsbTest() : message_loop_(new base::MessageLoop) {
    scoped_ptr<TestUsbMidiDeviceFactory> factory(new TestUsbMidiDeviceFactory);
    factory_ = factory.get();
    manager_.reset(new MidiManagerUsbForTesting(factory.Pass()));
  }
