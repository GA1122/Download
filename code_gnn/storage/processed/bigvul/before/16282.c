bool SafeSock :: init_MD(CONDOR_MD_MODE  , KeyInfo * key, const char * keyId)
{
    bool inited = true;
   
    if (mdChecker_) {   
        delete mdChecker_;
        mdChecker_ = 0;
    }
    if (key) {
        mdChecker_ = new Condor_MD_MAC(key);
    }

    if (_longMsg) {
        inited = _longMsg->verifyMD(mdChecker_);
    }
    else {
        inited = _shortMsg.verifyMD(mdChecker_);
    }

    if( !_outMsg.init_MD(keyId) ) {
        inited = false;
    }

    return inited;
}
