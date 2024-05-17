PHP_MINFO_FUNCTION(curl)
{
	curl_version_info_data *d;
	char **p;
	char str[1024];
	size_t n = 0;

	d = curl_version_info(CURLVERSION_NOW);
	php_info_print_table_start();
	php_info_print_table_row(2, "cURL support",    "enabled");
	php_info_print_table_row(2, "cURL Information", d->version);
	sprintf(str, "%d", d->age);
	php_info_print_table_row(2, "Age", str);

	 
	if (d->features) {
		struct feat {
			const char *name;
			int bitmask;
		};

		unsigned int i;

		static const struct feat feats[] = {
#if LIBCURL_VERSION_NUM >= 0x070a07  
			{"AsynchDNS", CURL_VERSION_ASYNCHDNS},
#endif
#if LIBCURL_VERSION_NUM >= 0x070f04  
			{"CharConv", CURL_VERSION_CONV},
#endif
#if LIBCURL_VERSION_NUM >= 0x070a06  
			{"Debug", CURL_VERSION_DEBUG},
			{"GSS-Negotiate", CURL_VERSION_GSSNEGOTIATE},
#endif
#if LIBCURL_VERSION_NUM >= 0x070c00  
			{"IDN", CURL_VERSION_IDN},
#endif
			{"IPv6", CURL_VERSION_IPV6},
			{"krb4", CURL_VERSION_KERBEROS4},
#if LIBCURL_VERSION_NUM >= 0x070b01  
			{"Largefile", CURL_VERSION_LARGEFILE},
#endif
			{"libz", CURL_VERSION_LIBZ},
#if LIBCURL_VERSION_NUM >= 0x070a06  
			{"NTLM", CURL_VERSION_NTLM},
#endif
#if LIBCURL_VERSION_NUM >= 0x071600  
			{"NTLMWB", CURL_VERSION_NTLM_WB},
#endif
#if LIBCURL_VERSION_NUM >= 0x070a08  
			{"SPNEGO", CURL_VERSION_SPNEGO},
#endif
			{"SSL",  CURL_VERSION_SSL},
#if LIBCURL_VERSION_NUM >= 0x070d02  
			{"SSPI",  CURL_VERSION_SSPI},
#endif
#if LIBCURL_VERSION_NUM >= 0x071504  
			{"TLS-SRP", CURL_VERSION_TLSAUTH_SRP},
#endif
#if LIBCURL_VERSION_NUM >= 0x072100  
			{"HTTP2", CURL_VERSION_HTTP2},
#endif
#if LIBCURL_VERSION_NUM >= 0x072600  
			{"GSSAPI", CURL_VERSION_GSSAPI},
#endif
			{NULL, 0}
		};

		php_info_print_table_row(1, "Features");
		for(i=0; i<sizeof(feats)/sizeof(feats[0]); i++) {
			if (feats[i].name) {
				php_info_print_table_row(2, feats[i].name, d->features & feats[i].bitmask ? "Yes" : "No");
			}
		}
	}

	n = 0;
	p = (char **) d->protocols;
	while (*p != NULL) {
			n += sprintf(str + n, "%s%s", *p, *(p + 1) != NULL ? ", " : "");
			p++;
	}
	php_info_print_table_row(2, "Protocols", str);

	php_info_print_table_row(2, "Host", d->host);

	if (d->ssl_version) {
		php_info_print_table_row(2, "SSL Version", d->ssl_version);
	}

	if (d->libz_version) {
		php_info_print_table_row(2, "ZLib Version", d->libz_version);
	}

#if defined(CURLVERSION_SECOND) && CURLVERSION_NOW >= CURLVERSION_SECOND
	if (d->ares) {
		php_info_print_table_row(2, "ZLib Version", d->ares);
	}
#endif

#if defined(CURLVERSION_THIRD) && CURLVERSION_NOW >= CURLVERSION_THIRD
	if (d->libidn) {
		php_info_print_table_row(2, "libIDN Version", d->libidn);
	}
#endif

#if LIBCURL_VERSION_NUM >= 0x071300

	if (d->iconv_ver_num) {
		php_info_print_table_row(2, "IconV Version", d->iconv_ver_num);
	}

	if (d->libssh_version) {
		php_info_print_table_row(2, "libSSH Version", d->libssh_version);
	}
#endif
	php_info_print_table_end();
}
