bool GDataDirectory::TakeEntry(GDataEntry* entry) {
  DCHECK(entry);
  DCHECK(entry->parent());

  entry->parent()->RemoveChild(entry);
  AddEntry(entry);

  return true;
}
