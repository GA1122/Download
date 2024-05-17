const char * SafeSock :: isIncomingDataEncrypted()
{
    char c;
    if (!peek(c)) {
        return 0;
    }
    else {
        if(_longMsg) {
            return _longMsg->isDataEncrypted();
        }
        else {  
            return _shortMsg.isDataEncrypted();
        }
    }
}
