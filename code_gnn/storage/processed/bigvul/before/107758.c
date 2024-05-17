Frame* FrameLoader::Opener() {
  return Client() ? Client()->Opener() : nullptr;
}
