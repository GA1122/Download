GDataEntry* GDataEntry::FromDocumentEntry(GDataDirectory* parent,
                                          DocumentEntry* doc,
                                          GDataRootDirectory* root) {
  DCHECK(doc);
  if (doc->is_folder())
    return GDataDirectory::FromDocumentEntry(parent, doc, root);
  else if (doc->is_hosted_document() || doc->is_file())
    return GDataFile::FromDocumentEntry(parent, doc, root);

  return NULL;
}
