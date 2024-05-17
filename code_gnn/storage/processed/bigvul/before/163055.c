void SandboxIPCHandler::HandleFontMatchRequest(
    int fd,
    base::PickleIterator iter,
    const std::vector<base::ScopedFD>& fds) {
  SkFontStyle requested_style;
  std::string family;
  if (!iter.ReadString(&family) ||
      !skia::ReadSkFontStyle(&iter, &requested_style))
    return;

  SkFontConfigInterface::FontIdentity result_identity;
  SkString result_family;
  SkFontStyle result_style;
  SkFontConfigInterface* fc =
      SkFontConfigInterface::GetSingletonDirectInterface();
  const bool r =
      fc->matchFamilyName(family.c_str(), requested_style, &result_identity,
                          &result_family, &result_style);

  base::Pickle reply;
  if (!r) {
    reply.WriteBool(false);
  } else {
    int index = FindOrAddPath(result_identity.fString);
    result_identity.fID = static_cast<uint32_t>(index);

    reply.WriteBool(true);
    skia::WriteSkString(&reply, result_family);
    skia::WriteSkFontIdentity(&reply, result_identity);
    skia::WriteSkFontStyle(&reply, result_style);
  }
  SendRendererReply(fds, reply, -1);
}
