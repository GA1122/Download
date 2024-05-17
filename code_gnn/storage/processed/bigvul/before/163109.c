void RecordBlobItemSizeStats(const DataElement& input_element) {
  uint64_t length = input_element.length();

  switch (input_element.type()) {
    case DataElement::TYPE_BYTES:
    case DataElement::TYPE_BYTES_DESCRIPTION:
      UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.Bytes", length / 1024);
      break;
    case DataElement::TYPE_BLOB:
      UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.Blob",
                              (length - input_element.offset()) / 1024);
      break;
    case DataElement::TYPE_FILE: {
      bool full_file = (length == std::numeric_limits<uint64_t>::max());
      UMA_HISTOGRAM_BOOLEAN("Storage.BlobItemSize.File.Unknown", full_file);
      if (!full_file) {
        UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.File",
                                (length - input_element.offset()) / 1024);
      }
      break;
    }
    case DataElement::TYPE_FILE_FILESYSTEM: {
      bool full_file = (length == std::numeric_limits<uint64_t>::max());
      UMA_HISTOGRAM_BOOLEAN("Storage.BlobItemSize.FileSystem.Unknown",
                            full_file);
      if (!full_file) {
        UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.FileSystem",
                                (length - input_element.offset()) / 1024);
      }
      break;
    }
    case DataElement::TYPE_DISK_CACHE_ENTRY:
      UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.CacheEntry",
                              (length - input_element.offset()) / 1024);
      break;
    case DataElement::TYPE_DATA_PIPE:
    case DataElement::TYPE_UNKNOWN:
      NOTREACHED();
      break;
  }
}
