void OSExchangeDataProviderAuraX11::SetURL(const GURL& url,
                                           const base::string16& title) {
  if (url.is_valid()) {
    base::string16 spec = base::UTF8ToUTF16(url.spec());

    std::vector<unsigned char> data;
    ui::AddString16ToVector(spec, &data);
    ui::AddString16ToVector(base::ASCIIToUTF16("\n"), &data);
    ui::AddString16ToVector(title, &data);
    scoped_refptr<base::RefCountedMemory> mem(
        base::RefCountedBytes::TakeVector(&data));

    format_map_.Insert(atom_cache_.GetAtom(kMimeTypeMozillaURL), mem);

    SetString(spec);
  }
}
