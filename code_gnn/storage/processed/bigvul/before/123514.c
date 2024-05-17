 bool IsFileType(BlobData::Item::Type type) {
   switch (type) {
     case BlobData::Item::TYPE_FILE:
    case BlobData::Item::TYPE_FILE_FILESYSTEM:
      return true;
    default:
      return false;
  }
}
