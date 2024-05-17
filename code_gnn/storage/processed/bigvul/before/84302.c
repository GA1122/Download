static DetectEngineThreadCtx *GetTenantById(HashTable *h, uint32_t id)
{
     
    return HashTableLookup(h, &id, 0);
}
