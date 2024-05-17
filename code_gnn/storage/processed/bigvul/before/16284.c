const char * SafeSock :: isIncomingDataMD5ed()
{
    char c;
    if (!peek(c)) {
        return 0;
    }
    else {
        if(_longMsg) {
            return _longMsg->isDataMD5ed();
        }
        else {  
            return _shortMsg.isDataMD5ed();
        }
    }
}
