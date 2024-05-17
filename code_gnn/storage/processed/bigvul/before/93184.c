dump_packet_and_trunc(u_char *user, const struct pcap_pkthdr *h, const u_char *sp)
{
	struct dump_info *dump_info;

	++packets_captured;

	++infodelay;

	dump_info = (struct dump_info *)user;

	 
	if (Gflag != 0) {
		 
		time_t t;

		 
		if ((t = time(NULL)) == (time_t)-1) {
			error("dump_and_trunc_packet: can't get current_time: %s",
			    pcap_strerror(errno));
		}


		 
		if (t - Gflag_time >= Gflag) {
#ifdef HAVE_CAPSICUM
			FILE *fp;
			int fd;
#endif

			 
			Gflag_time = t;
			 
			Gflag_count++;
			 
			pcap_dump_close(dump_info->p);

			 
			if (zflag != NULL)
				compress_savefile(dump_info->CurrentFileName);

			 
			if (Cflag == 0 && Wflag > 0 && Gflag_count >= Wflag) {
				(void)fprintf(stderr, "Maximum file limit reached: %d\n",
				    Wflag);
				info(1);
				exit_tcpdump(0);
				 
			}
			if (dump_info->CurrentFileName != NULL)
				free(dump_info->CurrentFileName);
			 
			dump_info->CurrentFileName = (char *)malloc(PATH_MAX + 1);
			if (dump_info->CurrentFileName == NULL)
				error("dump_packet_and_trunc: malloc");
			 
			Cflag_count = 0;

			 
			if (Cflag != 0)
				MakeFilename(dump_info->CurrentFileName, dump_info->WFileName, 0,
				    WflagChars);
			else
				MakeFilename(dump_info->CurrentFileName, dump_info->WFileName, 0, 0);

#ifdef HAVE_LIBCAP_NG
			capng_update(CAPNG_ADD, CAPNG_EFFECTIVE, CAP_DAC_OVERRIDE);
			capng_apply(CAPNG_SELECT_BOTH);
#endif  
#ifdef HAVE_CAPSICUM
			fd = openat(dump_info->dirfd,
			    dump_info->CurrentFileName,
			    O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0) {
				error("unable to open file %s",
				    dump_info->CurrentFileName);
			}
			fp = fdopen(fd, "w");
			if (fp == NULL) {
				error("unable to fdopen file %s",
				    dump_info->CurrentFileName);
			}
			dump_info->p = pcap_dump_fopen(dump_info->pd, fp);
#else	 
			dump_info->p = pcap_dump_open(dump_info->pd, dump_info->CurrentFileName);
#endif
#ifdef HAVE_LIBCAP_NG
			capng_update(CAPNG_DROP, CAPNG_EFFECTIVE, CAP_DAC_OVERRIDE);
			capng_apply(CAPNG_SELECT_BOTH);
#endif  
			if (dump_info->p == NULL)
				error("%s", pcap_geterr(pd));
#ifdef HAVE_CAPSICUM
			set_dumper_capsicum_rights(dump_info->p);
#endif
		}
	}

	 
	if (Cflag != 0) {
		long size = pcap_dump_ftell(dump_info->p);

		if (size == -1)
			error("ftell fails on output file");
		if (size > Cflag) {
#ifdef HAVE_CAPSICUM
			FILE *fp;
			int fd;
#endif

			 
			pcap_dump_close(dump_info->p);

			 
			if (zflag != NULL)
				compress_savefile(dump_info->CurrentFileName);

			Cflag_count++;
			if (Wflag > 0) {
				if (Cflag_count >= Wflag)
					Cflag_count = 0;
			}
			if (dump_info->CurrentFileName != NULL)
				free(dump_info->CurrentFileName);
			dump_info->CurrentFileName = (char *)malloc(PATH_MAX + 1);
			if (dump_info->CurrentFileName == NULL)
				error("dump_packet_and_trunc: malloc");
			MakeFilename(dump_info->CurrentFileName, dump_info->WFileName, Cflag_count, WflagChars);
#ifdef HAVE_LIBCAP_NG
			capng_update(CAPNG_ADD, CAPNG_EFFECTIVE, CAP_DAC_OVERRIDE);
			capng_apply(CAPNG_SELECT_BOTH);
#endif  
#ifdef HAVE_CAPSICUM
			fd = openat(dump_info->dirfd, dump_info->CurrentFileName,
			    O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0) {
				error("unable to open file %s",
				    dump_info->CurrentFileName);
			}
			fp = fdopen(fd, "w");
			if (fp == NULL) {
				error("unable to fdopen file %s",
				    dump_info->CurrentFileName);
			}
			dump_info->p = pcap_dump_fopen(dump_info->pd, fp);
#else	 
			dump_info->p = pcap_dump_open(dump_info->pd, dump_info->CurrentFileName);
#endif
#ifdef HAVE_LIBCAP_NG
			capng_update(CAPNG_DROP, CAPNG_EFFECTIVE, CAP_DAC_OVERRIDE);
			capng_apply(CAPNG_SELECT_BOTH);
#endif  
			if (dump_info->p == NULL)
				error("%s", pcap_geterr(pd));
#ifdef HAVE_CAPSICUM
			set_dumper_capsicum_rights(dump_info->p);
#endif
		}
	}

	pcap_dump((u_char *)dump_info->p, h, sp);
#ifdef HAVE_PCAP_DUMP_FLUSH
	if (Uflag)
		pcap_dump_flush(dump_info->p);
#endif

	--infodelay;
	if (infoprint)
		info(0);
}
