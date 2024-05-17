static int backend_version(struct backend *be)
{
    const char *minor;

     

     
    if (strstr(be->banner, cyrus_version())) {
        return MAILBOX_MINOR_VERSION;
    }

     
    if (strstr(be->banner, "Cyrus IMAP 3.0")) {
        return 13;
    }

     
    if (strstr(be->banner, "Cyrus IMAP 2.5.")
     || strstr(be->banner, "Cyrus IMAP Murder 2.5.")
     || strstr(be->banner, "git2.5.")) {
        return 13;
    }

     
    if (strstr(be->banner, "v2.4.") || strstr(be->banner, "git2.4.")) {
        return 12;
    }

    minor = strstr(be->banner, "v2.3.");
    if (!minor) return 6;

     
    if (minor[1] != ' ') {
        return 10;
    }
     
    switch (minor[0]) {
    case '0':
    case '1':
    case '2':
    case '3':
        return 7;
        break;

    case '4':
    case '5':
    case '6':
        return 8;
        break;

    case '7':
    case '8':
    case '9':
        return 9;
        break;
    }

     
    return 6;
}
