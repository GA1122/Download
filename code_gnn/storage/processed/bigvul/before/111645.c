GDataEntry::GDataEntry(GDataDirectory* parent, GDataRootDirectory* root)
    : root_(root),
      deleted_(false) {
  SetParent(parent);
}
