bool GDataDirectory::RemoveEntry(GDataEntry* entry) {
  DCHECK(entry);

  if (!RemoveChild(entry))
    return false;

  delete entry;
  return true;
}
