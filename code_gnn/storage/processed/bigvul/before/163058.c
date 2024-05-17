void SandboxIPCHandler::HandleGetStyleForStrike(
    int fd,
    base::PickleIterator iter,
    const std::vector<base::ScopedFD>& fds) {
  std::string family;
  bool bold;
  bool italic;
  uint16_t pixel_size;

  if (!iter.ReadString(&family) || !iter.ReadBool(&bold) ||
      !iter.ReadBool(&italic) || !iter.ReadUInt16(&pixel_size)) {
    return;
  }

  gfx::FontRenderParamsQuery query;
  query.families.push_back(family);
  query.pixel_size = pixel_size;
  query.style = italic ? gfx::Font::ITALIC : 0;
  query.weight = bold ? gfx::Font::Weight::BOLD : gfx::Font::Weight::NORMAL;
  const gfx::FontRenderParams params = gfx::GetFontRenderParams(query, nullptr);

  base::Pickle reply;
  reply.WriteInt(params.use_bitmaps);
  reply.WriteInt(params.autohinter);
  reply.WriteInt(params.hinting != gfx::FontRenderParams::HINTING_NONE);
  reply.WriteInt(ConvertHinting(params.hinting));
  reply.WriteInt(params.antialiasing);
  reply.WriteInt(ConvertSubpixelRendering(params.subpixel_rendering));
  reply.WriteInt(params.subpixel_positioning);

  SendRendererReply(fds, reply, -1);
}
