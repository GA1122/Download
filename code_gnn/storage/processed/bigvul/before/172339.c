bool omx_video::execute_omx_flush(OMX_U32 flushType)
{
 bool bRet = false;
    DEBUG_PRINT_LOW("execute_omx_flush -  %u", (unsigned int)flushType);
#ifdef _MSM8974_
  

    input_flush_progress = true;
    output_flush_progress = true;
    bRet = execute_flush_all();
#else
 if (flushType == 0 || flushType == OMX_ALL) {
        input_flush_progress = true;
        bRet = execute_input_flush();
 }
 if (flushType == 1 || flushType == OMX_ALL) {
        output_flush_progress = true;
        bRet = execute_output_flush();
 }
#endif
 return bRet;
}
