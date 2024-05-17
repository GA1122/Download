static int auth_sasl_check_ssf(RedsSASL *sasl, int *runSSF)
{
    const void *val;
    int err, ssf;

    *runSSF = 0;
    if (!sasl->wantSSF) {
        return 1;
    }

    err = sasl_getprop(sasl->conn, SASL_SSF, &val);
    if (err != SASL_OK) {
        return 0;
    }

    ssf = *(const int *)val;
    spice_info("negotiated an SSF of %d", ssf);
    if (ssf < 56) {
        return 0;  
    }

    *runSSF = 1;

     
    return 1;
}
