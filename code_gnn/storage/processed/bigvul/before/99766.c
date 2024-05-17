void Vp9Parser::ReadDisplayFrameSize(Vp9FrameHeader* fhdr) {
  if (reader_.ReadBool()) {
    fhdr->display_width = reader_.ReadLiteral(16) + 1;
    fhdr->display_height = reader_.ReadLiteral(16) + 1;
  } else {
    fhdr->display_width = fhdr->width;
    fhdr->display_height = fhdr->height;
  }
}
