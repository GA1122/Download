const GDataDirectory* GDataEntry::AsGDataDirectoryConst() const {
  return const_cast<GDataEntry*>(this)->AsGDataDirectory();
}
