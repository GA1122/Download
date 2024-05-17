long long Cluster::GetLastTime() const {
 const BlockEntry* pEntry;

 const long status = GetLast(pEntry);

 if (status < 0)  
 return status;

 if (pEntry == NULL)  
 return GetTime();

 const Block* const pBlock = pEntry->GetBlock();
  assert(pBlock);

 return pBlock->GetTime(this);
}
