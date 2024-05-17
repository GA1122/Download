void BlobStorageContext::CancelBuildingBlob(const std::string& uuid,
                                            BlobStatus reason) {
  CancelBuildingBlobInternal(registry_.GetEntry(uuid), reason);
}
