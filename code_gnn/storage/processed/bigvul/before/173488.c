bool omx_vdec::release_done(void)
{
 bool bRet = false;

 if (release_input_done()) {
 if (release_output_done()) {
            bRet = true;
 }
 }
 return bRet;
}
