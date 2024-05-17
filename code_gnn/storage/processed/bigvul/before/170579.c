void LvmEffect_free(EffectContext *pContext){
    LVM_ReturnStatus_en     LvmStatus=LVM_SUCCESS;  
 LVM_ControlParams_t     params;  
 LVM_MemTab_t MemTab;

  
 LvmStatus = LVM_GetMemoryTable(pContext->pBundledContext->hInstance,
 &MemTab,
                                   LVM_NULL);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetMemoryTable", "LvmEffect_free")

 for (int i=0; i<LVM_NR_MEMORY_REGIONS; i++){
 if (MemTab.Region[i].Size != 0){
 if (MemTab.Region[i].pBaseAddress != NULL){
                ALOGV("\tLvmEffect_free - START freeing %" PRIu32 " bytes for region %u at %p\n",
 MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);

                free(MemTab.Region[i].pBaseAddress);

                ALOGV("\tLvmEffect_free - END   freeing %" PRIu32 " bytes for region %u at %p\n",
 MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
 }else{
                ALOGV("\tLVM_ERROR : LvmEffect_free - trying to free with NULL pointer %" PRIu32
 " bytes for region %u at %p ERROR\n",
 MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
 }
 }
 }
}  
