bool omx_video::allocate_done(void)
{
 bool bRet = false;
 bool bRet_In = false;
 bool bRet_Out = false;

    bRet_In = allocate_input_done();
    bRet_Out = allocate_output_done();

 if (bRet_In && bRet_Out) {
        bRet = true;
 }

 return bRet;
}
