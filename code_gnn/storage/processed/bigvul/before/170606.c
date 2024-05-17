int32_t ReverbGetDensity(ReverbContext *pContext){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  
    LVM_INT16                 Temp;
  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbGetDensity")


 Temp = (LVM_INT16)(((pContext->SavedDensity * 99) / 1000) + 1);

 if(Temp != ActiveParams.RoomSize){
        ALOGV("\tLVM_ERROR : ReverbGetDensity invalid value %d %d", Temp, ActiveParams.RoomSize);
 }

 return pContext->SavedDensity;
}
