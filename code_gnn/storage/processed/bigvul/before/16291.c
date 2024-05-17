int SafeSock::put_bytes(const void *data, int sz)
{
	int bytesPut, l_out;
    unsigned char * dta = 0;


    if (get_encryption()) {
        if (!wrap((unsigned char *)data, sz, dta , l_out)) { 
            dprintf(D_SECURITY, "Encryption failed\n");
            return -1;   
        }
    }
    else {
        dta = (unsigned char *) malloc(sz);
        memcpy(dta, data, sz);
    }
    
    if (mdChecker_) {
        mdChecker_->addMD(dta, sz);
    }


    bytesPut = _outMsg.putn((char *)dta, sz);
    
    free(dta);
    
	return bytesPut;
}
