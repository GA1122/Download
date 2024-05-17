static EAS_RESULT ReadDLSID (SDLS_SYNTHESIZER_DATA *pDLSData, DLSID *pDLSID)
{
    EAS_RESULT result;
    EAS_I32 n;

 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &pDLSID->Data1, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &pDLSID->Data2, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &pDLSID->Data3, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 return EAS_HWReadFile(pDLSData->hwInstData, pDLSData->fileHandle, pDLSID->Data4, sizeof(pDLSID->Data4), &n);
}
