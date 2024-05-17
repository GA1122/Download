static int _etc_hosts_lookup(const char *cszName, char *szIP, const int ciMaxIPLen) {
#define EHL_LINE_LEN 260
    int iSuccess = 0;
    size_t iLen;
    char szLine[EHL_LINE_LEN + 1];  
    char *pcStart, *pcEnd;
    FILE *fHosts;

    do {
         
        fHosts = NULL;

         
        if ((cszName == NULL) || (szIP == NULL) || (ciMaxIPLen <= 0))
            break;
        szIP[0] = 0;

         
#ifdef _WIN32
        pcStart = getenv("WINDIR");
        if (pcStart != NULL) {
            sprintf(szLine, "%s\\system32\\drivers\\etc\\hosts", pcStart);
        } else {
            strcpy(szLine, "C:\\WINDOWS\\system32\\drivers\\etc\\hosts");
        }
#else
        strcpy(szLine, "/etc/hosts");
#endif
        fHosts = fopen(szLine, "r");
        if (fHosts == NULL)
            break;

         
        while (fgets(szLine, EHL_LINE_LEN, fHosts) != NULL) {
             
            pcStart = strchr (szLine, '#');
            if (pcStart != NULL)
                *pcStart = 0;
            strcat(szLine, " ");  

             
            iLen = strspn(szLine, "1234567890.");
            if ((iLen < 7) || (iLen > 15))  
                continue;
            pcEnd = szLine + iLen;
            *pcEnd = 0;
            pcEnd++;  

             
            pcStart = pcEnd + strspn(pcEnd, " \t\n");
            while (*pcStart != 0) {
                pcEnd = pcStart + strcspn(pcStart, " \t\n");
                *pcEnd = 0;
                pcEnd++;  

                if (strcasecmp(pcStart, cszName) == 0) {
                    strncpy(szIP, szLine, ciMaxIPLen - 1);
                    szIP[ciMaxIPLen - 1] = '\0';
                    iSuccess = 1;
                    break;
                }

                pcStart = pcEnd + strspn(pcEnd, " \t\n");
            }
            if (iSuccess)
                break;
        }
    } while (0);

    if (fHosts != NULL)
        fclose(fHosts);

    return (iSuccess);
}
