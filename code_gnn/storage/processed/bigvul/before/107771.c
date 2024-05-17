void FrameLoader::SetOpener(LocalFrame* opener) {
  if (Client())
    Client()->SetOpener(opener);
}
