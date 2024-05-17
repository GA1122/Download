bool HFSIterator::Open() {
  if (stream_->Seek(1024, SEEK_SET) != 1024)
    return false;

  if (!stream_->ReadType(&volume_header_)) {
    DLOG(ERROR) << "Failed to read volume header";
    return false;
  }
  ConvertBigEndian(&volume_header_);

  if (volume_header_.signature != kHFSPlusSigWord &&
      volume_header_.signature != kHFSXSigWord) {
    DLOG(ERROR) << "Unrecognized volume header signature "
                << volume_header_.signature;
    return false;
  }

  if (!ReadCatalogFile())
    return false;

  return true;
}
