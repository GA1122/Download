void OSExchangeDataProviderAuraX11::SetHtml(const base::string16& html,
                                            const GURL& base_url) {
  std::vector<unsigned char> bytes;
  bytes.push_back(0xFF);
  bytes.push_back(0xFE);
  ui::AddString16ToVector(html, &bytes);
  scoped_refptr<base::RefCountedMemory> mem(
      base::RefCountedBytes::TakeVector(&bytes));

  format_map_.Insert(atom_cache_.GetAtom(Clipboard::kMimeTypeHTML), mem);
}
