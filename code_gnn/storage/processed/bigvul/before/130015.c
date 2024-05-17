bool VariationsSeedStore::ApplyDeltaPatch(const std::string& existing_data,
                                          const std::string& patch,
                                          std::string* output) {
  output->clear();

  google::protobuf::io::CodedInputStream in(
      reinterpret_cast<const uint8*>(patch.data()), patch.length());
  std::string temp;

  const uint32 existing_data_size = static_cast<uint32>(existing_data.size());
  while (in.CurrentPosition() != static_cast<int>(patch.length())) {
    uint32 value;
    if (!in.ReadVarint32(&value))
      return false;

    if (value != 0) {

      if (!in.ReadString(&temp, value))
        return false;
      output->append(temp);
    } else {
      uint32 offset;
      uint32 length;
      if (!in.ReadVarint32(&offset) || !in.ReadVarint32(&length))
        return false;

      base::CheckedNumeric<uint32> end_offset(offset);
      end_offset += length;
      if (!end_offset.IsValid() || end_offset.ValueOrDie() > existing_data_size)
        return false;
      output->append(existing_data, offset, length);
    }
  }
  return true;
}
