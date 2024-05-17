void MidiManagerUsb::StartInitialization() {
  Initialize(
      base::Bind(&MidiManager::CompleteInitialization, base::Unretained(this)));
}
