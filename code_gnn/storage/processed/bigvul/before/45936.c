do_core_note(struct magic_set *ms, unsigned char *nbuf, uint32_t type,
    int swap, uint32_t namesz, uint32_t descsz,
    size_t noff, size_t doff, int *flags, size_t size, int clazz)
{
#ifdef ELFCORE
	int os_style = -1;
	 
	if ((namesz == 4 && strncmp((char *)&nbuf[noff], "CORE", 4) == 0) ||
	    (namesz == 5 && strcmp((char *)&nbuf[noff], "CORE") == 0)) {
		os_style = OS_STYLE_SVR4;
	} 

	if ((namesz == 8 && strcmp((char *)&nbuf[noff], "FreeBSD") == 0)) {
		os_style = OS_STYLE_FREEBSD;
	}

	if ((namesz >= 11 && strncmp((char *)&nbuf[noff], "NetBSD-CORE", 11)
	    == 0)) {
		os_style = OS_STYLE_NETBSD;
	}

	if (os_style != -1 && (*flags & FLAGS_DID_CORE_STYLE) == 0) {
		if (file_printf(ms, ", %s-style", os_style_names[os_style])
		    == -1)
			return 1;
		*flags |= FLAGS_DID_CORE_STYLE;
	}

	switch (os_style) {
	case OS_STYLE_NETBSD:
		if (type == NT_NETBSD_CORE_PROCINFO) {
			char sbuf[512];
			uint32_t signo;
			 
			if (file_printf(ms, ", from '%.31s'",
			    file_printable(sbuf, sizeof(sbuf),
			    (const char *)&nbuf[doff + 0x7c])) == -1)
				return 1;
			
			 
			(void)memcpy(&signo, &nbuf[doff + 0x08],
			    sizeof(signo));
			if (file_printf(ms, " (signal %u)",
			    elf_getu32(swap, signo)) == -1)
				return 1;
			*flags |= FLAGS_DID_CORE;
			return 1;
		}
		break;

	default:
		if (type == NT_PRPSINFO && *flags & FLAGS_IS_CORE) {
			size_t i, j;
			unsigned char c;
			 
			for (i = 0; i < NOFFSETS; i++) {
				unsigned char *cname, *cp;
				size_t reloffset = prpsoffsets(i);
				size_t noffset = doff + reloffset;
				size_t k;
				for (j = 0; j < 16; j++, noffset++,
				    reloffset++) {
					 
					if (noffset >= size)
						goto tryanother;

					 
					if (reloffset >= descsz)
						goto tryanother;

					c = nbuf[noffset];
					if (c == '\0') {
						 
						if (j == 0)
							goto tryanother;
						else
							break;
					} else {
						 
						if (!isprint(c) || isquote(c))
							goto tryanother;
					}
				}
				 

				 
				for (k = i + 1 ; k < NOFFSETS; k++) {
					size_t no;
					int adjust = 1;
					if (prpsoffsets(k) >= prpsoffsets(i))
						continue;
					for (no = doff + prpsoffsets(k);
					     no < doff + prpsoffsets(i); no++)
						adjust = adjust
						         && isprint(nbuf[no]);
					if (adjust)
						i = k;
				}

				cname = (unsigned char *)
				    &nbuf[doff + prpsoffsets(i)];
				for (cp = cname; *cp && isprint(*cp); cp++)
					continue;
				 
				while (cp > cname && isspace(cp[-1]))
					cp--;
				if (file_printf(ms, ", from '%.*s'",
				    (int)(cp - cname), cname) == -1)
					return 1;
				*flags |= FLAGS_DID_CORE;
				return 1;

			tryanother:
				;
			}
		}
		break;
	}
#endif
	return 0;
}
