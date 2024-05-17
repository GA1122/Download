void OSExchangeDataProviderAuraX11::SetString(const base::string16& text_data) {
  std::string utf8 = base::UTF16ToUTF8(text_data);
  scoped_refptr<base::RefCountedMemory> mem(
      base::RefCountedString::TakeString(&utf8));

  format_map_.Insert(atom_cache_.GetAtom(Clipboard::kMimeTypeText), mem);
  format_map_.Insert(atom_cache_.GetAtom(kText), mem);
  format_map_.Insert(atom_cache_.GetAtom(kString), mem);
  format_map_.Insert(atom_cache_.GetAtom(kUtf8String), mem);
}
