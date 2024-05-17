void BookmarksIOFunction::MultiFilesSelected(
    const std::vector<base::FilePath>& files, void* params) {
  Release();   
  NOTREACHED() << "Should not be able to select multiple files";
}
