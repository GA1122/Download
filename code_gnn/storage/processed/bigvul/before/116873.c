WebKit::WebAudioDevice* TestWebKitPlatformSupport::createAudioDevice(
    size_t bufferSize, unsigned numberOfChannels, double sampleRate,
    WebKit::WebAudioDevice::RenderCallback*) {
  return new WebAudioDeviceMock(sampleRate);
}
