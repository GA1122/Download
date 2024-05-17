static void InitOMXParams(T *params) {
    params->nSize = sizeof(T);
    params->nVersion.s.nVersionMajor = 1;
    params->nVersion.s.nVersionMinor = 1;
    params->nVersion.s.nRevision = 2;
    params->nVersion.s.nStep = 0;
}
