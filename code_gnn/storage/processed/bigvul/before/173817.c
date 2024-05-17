void SoftGSM::onPortFlushCompleted(OMX_U32 portIndex) {
 if (portIndex == 0) {
        gsm_destroy(mGsm);
        mGsm = gsm_create();
 int msopt = 1;
        gsm_option(mGsm, GSM_OPT_WAV49, &msopt);
 }
}
