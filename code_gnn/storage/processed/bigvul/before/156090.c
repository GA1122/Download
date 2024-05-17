void StripTrailingDot(base::StringPiece* host) {
  if (host->ends_with("."))
    host->remove_suffix(1);
}
