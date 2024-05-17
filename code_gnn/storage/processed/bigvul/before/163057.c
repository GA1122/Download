void SandboxIPCHandler::HandleGetFallbackFontForChar(
    int fd,
    base::PickleIterator iter,
    const std::vector<base::ScopedFD>& fds) {

  UChar32 c;
  if (!iter.ReadInt(&c))
    return;

  std::string preferred_locale;
  if (!iter.ReadString(&preferred_locale))
    return;

  auto fallback_font = gfx::GetFallbackFontForChar(c, preferred_locale);
  int fontconfig_interface_id =
      FindOrAddPath(SkString(fallback_font.filename.data()));

  if (g_test_observer) {
    g_test_observer->OnGetFallbackFontForChar(c, fallback_font.name,
                                              fontconfig_interface_id);
  }
  base::Pickle reply;
  reply.WriteString(fallback_font.name);
  reply.WriteString(fallback_font.filename);
  reply.WriteInt(fontconfig_interface_id);
  reply.WriteInt(fallback_font.ttc_index);
  reply.WriteBool(fallback_font.is_bold);
  reply.WriteBool(fallback_font.is_italic);
  SendRendererReply(fds, reply, -1);
}
