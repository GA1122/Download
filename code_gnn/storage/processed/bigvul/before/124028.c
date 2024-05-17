void BookmarksImportFunction::FileSelected(const base::FilePath& path,
                                           int index,
                                           void* params) {
#if !defined(OS_ANDROID)
  scoped_refptr<ImporterHost> importer_host(new ImporterHost);
  importer::SourceProfile source_profile;
  source_profile.importer_type = importer::TYPE_BOOKMARKS_FILE;
  source_profile.source_path = path;
  importer_host->StartImportSettings(source_profile,
                                     profile(),
                                     importer::FAVORITES,
                                     new ProfileWriter(profile()),
                                     true);
#endif
  Release();   
}
