void SoftGSM::onReset() {
    gsm_destroy(mGsm);
    mGsm = gsm_create();
 int msopt = 1;
    gsm_option(mGsm, GSM_OPT_WAV49, &msopt);
    mSignalledError = false;
}
