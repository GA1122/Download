void OSExchangeDataProviderAuraX11::SetFilenames(
    const std::vector<OSExchangeData::FileInfo>& filenames) {
  std::vector<std::string> paths;
  for (std::vector<OSExchangeData::FileInfo>::const_iterator it =
           filenames.begin(); it != filenames.end(); ++it) {
    std::string url_spec = net::FilePathToFileURL(it->path).spec();
    if (!url_spec.empty())
      paths.push_back(url_spec);
  }

  std::string joined_data = JoinString(paths, '\n');
  scoped_refptr<base::RefCountedMemory> mem(
      base::RefCountedString::TakeString(&joined_data));
  format_map_.Insert(atom_cache_.GetAtom(Clipboard::kMimeTypeURIList), mem);
}
