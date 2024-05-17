Class2Modem::Class2Modem(FaxServer& s, const ModemConfig& c) : FaxModem(s,c)
{
    hangupCode[0] = '\0';
    serviceType = 0;			 
    useExtendedDF = false;		 
    useJP = false;			 
    imagefd = 0;
}
