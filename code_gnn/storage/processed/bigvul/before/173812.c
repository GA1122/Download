SoftGSM::SoftGSM(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mSignalledError(false) {

    CHECK(!strcmp(name, "OMX.google.gsm.decoder"));

    mGsm = gsm_create();
    CHECK(mGsm);
 int msopt = 1;
    gsm_option(mGsm, GSM_OPT_WAV49, &msopt);

    initPorts();
}
