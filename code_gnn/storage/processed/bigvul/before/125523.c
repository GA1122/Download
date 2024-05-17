void GDataDirectory::RemoveEntry(GDataEntry* entry) {
  DCHECK(entry);

  RemoveChild(entry);
  delete entry;
}
