void OSExchangeDataProviderAuraX11::SetPickledData(
    const OSExchangeData::CustomFormat& format,
    const Pickle& pickle) {
  const unsigned char* data =
      reinterpret_cast<const unsigned char*>(pickle.data());

  std::vector<unsigned char> bytes;
  bytes.insert(bytes.end(), data, data + pickle.size());
  scoped_refptr<base::RefCountedMemory> mem(
      base::RefCountedBytes::TakeVector(&bytes));

  format_map_.Insert(atom_cache_.GetAtom(format.ToString().c_str()), mem);
}
