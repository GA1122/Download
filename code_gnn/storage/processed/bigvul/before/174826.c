void Reverb_free(ReverbContext *pContext){

    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  
    LVREV_ControlParams_st    params;  
    LVREV_MemoryTable_st      MemTab;

  
 LvmStatus = LVREV_GetMemoryTable(pContext->hInstance,
 &MemTab,
                                   LVM_NULL);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetMemoryTable", "Reverb_free")

 for (int i=0; i<LVM_NR_MEMORY_REGIONS; i++){
 if (MemTab.Region[i].Size != 0){
 if (MemTab.Region[i].pBaseAddress != NULL){
                ALOGV("\tfree() - START freeing %ld bytes for region %u at %p\n",
 MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);

                free(MemTab.Region[i].pBaseAddress);

                ALOGV("\tfree() - END   freeing %ld bytes for region %u at %p\n",
 MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
 }else{
                ALOGV("\tLVM_ERROR : free() - trying to free with NULL pointer %ld bytes "
 "for region %u at %p ERROR\n",
 MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
 }
 }
 }
}  
