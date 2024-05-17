bool GlobalHistogramAllocator::WriteToPersistentLocation() {
#if defined(OS_NACL)
  NOTREACHED();
  return false;
#else
  if (persistent_location_.empty()) {
    NOTREACHED() << "Could not write \"" << Name() << "\" persistent histograms"
                 << " to file because no location was set.";
    return false;
  }

  StringPiece contents(static_cast<const char*>(data()), used());
  if (!ImportantFileWriter::WriteFileAtomically(persistent_location_,
                                                contents)) {
    LOG(ERROR) << "Could not write \"" << Name() << "\" persistent histograms"
               << " to file: " << persistent_location_.value();
    return false;
  }

  return true;
#endif
}
