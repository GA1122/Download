bool HFSBTreeIterator::Next() {
  if (!ReadCurrentLeaf())
    return false;

  GetLeafData<uint16_t>();   
  auto parent_id = OSSwapBigToHostInt32(*GetLeafData<uint32_t>());
  auto key_string_length = OSSwapBigToHostInt16(*GetLeafData<uint16_t>());
  auto key_string =
      reinterpret_cast<uint16_t*>(&leaf_data_[current_leaf_offset_]);
  for (uint16_t i = 0;
       i < key_string_length;
       ++i, current_leaf_offset_ += sizeof(uint16_t)) {
    key_string[i] = OSSwapBigToHostInt16(key_string[i]);
  }
  base::string16 key(key_string, key_string_length);

  current_record_.record_type = OSSwapBigToHostInt16(*GetLeafData<int16_t>());
  current_record_.unexported = false;
  current_leaf_offset_ -= sizeof(int16_t);
  switch (current_record_.record_type) {
    case kHFSPlusFolderRecord: {
      auto folder = GetLeafData<HFSPlusCatalogFolder>();
      ConvertBigEndian(folder);
      ++leaf_records_read_;
      ++current_leaf_records_read_;

      if (IsKeyUnexported(key) ||
          unexported_parents_.find(parent_id) != unexported_parents_.end()) {
        unexported_parents_.insert(folder->folderID);
        current_record_.unexported = true;
      }

      if (parent_id != 0) {
        auto parent_name = folder_cnid_map_.find(parent_id);
        if (parent_name != folder_cnid_map_.end())
          key = parent_name->second + kFilePathSeparator + key;
      }
      folder_cnid_map_[folder->folderID] = key;

      current_record_.path = key;
      current_record_.folder = folder;
      break;
    }
    case kHFSPlusFileRecord: {
      auto file = GetLeafData<HFSPlusCatalogFile>();
      ConvertBigEndian(file);
      ++leaf_records_read_;
      ++current_leaf_records_read_;

      base::string16 path =
          folder_cnid_map_[parent_id] + kFilePathSeparator + key;
      current_record_.path = path;
      current_record_.file = file;
      current_record_.unexported =
          unexported_parents_.find(parent_id) != unexported_parents_.end();
      break;
    }
    case kHFSPlusFolderThreadRecord:
    case kHFSPlusFileThreadRecord: {
      GetLeafData<uint16_t>();   
      GetLeafData<uint16_t>();   
      GetLeafData<uint32_t>();   
      auto string_length = OSSwapBigToHostInt16(*GetLeafData<uint16_t>());
      for (uint16_t i = 0; i < string_length; ++i)
        GetLeafData<uint16_t>();
      ++leaf_records_read_;
      ++current_leaf_records_read_;
      break;
    }
    default:
      DLOG(ERROR) << "Unknown record type " << current_record_.record_type;
      return false;
  }

  if (current_leaf_records_read_ >= current_leaf_->numRecords) {
    current_leaf_number_ = current_leaf_->fLink;
    read_current_leaf_ = false;
  }

  return true;
}
