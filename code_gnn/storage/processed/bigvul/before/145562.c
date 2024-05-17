ProtocolVersion ConvertStringToProtocolVersion(base::StringPiece version) {
  if (version == kCtap2Version)
    return ProtocolVersion::kCtap;
  if (version == kU2fVersion)
    return ProtocolVersion::kU2f;

  return ProtocolVersion::kUnknown;
}
