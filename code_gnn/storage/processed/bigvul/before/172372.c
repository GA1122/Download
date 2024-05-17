bool omx_video::release_done(void)
{
 bool bRet = false;
    DEBUG_PRINT_LOW("Inside release_done()");
 if (release_input_done()) {
 if (release_output_done()) {
            bRet = true;
 }
 }
 return bRet;
}
