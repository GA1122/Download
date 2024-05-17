  void AddURLList(const std::string& list_contents) {
    std::string contents_copy = list_contents;
    scoped_refptr<base::RefCountedMemory> mem(
        base::RefCountedString::TakeString(&contents_copy));

    provider.format_map_.Insert(
        provider.atom_cache_.GetAtom(ui::Clipboard::kMimeTypeURIList),
        mem);
  }
