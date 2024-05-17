void Vp9Parser::ReadFrameSize(Vp9FrameHeader* fhdr) {
  fhdr->width = reader_.ReadLiteral(16) + 1;
  fhdr->height = reader_.ReadLiteral(16) + 1;
}
