uint8_t Vp9Parser::ReadProfile() {
  uint8_t profile = 0;

  if (reader_.ReadBool())
    profile |= 1;
  if (reader_.ReadBool())
    profile |= 2;
  if (profile > 2 && reader_.ReadBool())
    profile += 1;
  return profile;
}
