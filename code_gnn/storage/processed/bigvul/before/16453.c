int sysapi_translate_opsys_version( 
    const char *  ,
	const char *release,
	const char *   )
{
    const char * psz = release;

    while (psz[0] && (psz[0] < '0' || psz[0] > '9')) {
       ++psz;
    }

    int major = 0;
    while (psz[0]) {
        if (psz[0] >= '0' && psz[0] <= '9') {
            major = major * 10 + (psz[0] - '0');
        } else {
           break;
        }
        ++psz;
    }

    int minor = 0;
    if (psz[0] == '.') {
       ++psz;
       if (psz[0] >= '0' && psz[0] <= '9') {
          minor = psz[0] - '0';
          ++psz;
       }
       if (psz[0] >= '0' && psz[0] <= '9') {
          minor = minor * 10 + psz[0] - '0';
       }
    }

    return (major * 100) + minor;
}
