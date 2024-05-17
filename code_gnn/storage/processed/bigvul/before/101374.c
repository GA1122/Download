int64 BaseNode::GetId() const {
   return GetEntry()->Get(syncable::META_HANDLE);
 }
