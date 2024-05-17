static void ConvertBigEndian(HFSPlusCatalogFolder* folder) {
  ConvertBigEndian(&folder->recordType);
  ConvertBigEndian(&folder->flags);
  ConvertBigEndian(&folder->valence);
  ConvertBigEndian(&folder->folderID);
  ConvertBigEndian(&folder->createDate);
  ConvertBigEndian(&folder->contentModDate);
  ConvertBigEndian(&folder->attributeModDate);
  ConvertBigEndian(&folder->accessDate);
  ConvertBigEndian(&folder->backupDate);
  ConvertBigEndian(&folder->bsdInfo.ownerID);
  ConvertBigEndian(&folder->bsdInfo.groupID);
  ConvertBigEndian(&folder->bsdInfo.fileMode);
  ConvertBigEndian(&folder->textEncoding);
  ConvertBigEndian(&folder->folderCount);
}
