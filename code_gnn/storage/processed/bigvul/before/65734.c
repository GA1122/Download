static __be32 nfsd4_decode_cb_sec(struct nfsd4_compoundargs *argp, struct nfsd4_cb_sec *cbs)
{
	DECODE_HEAD;
	u32 dummy, uid, gid;
	char *machine_name;
	int i;
	int nr_secflavs;

	 
	READ_BUF(4);
	nr_secflavs = be32_to_cpup(p++);
	if (nr_secflavs)
		cbs->flavor = (u32)(-1);
	else
		 
		cbs->flavor = 0;
	for (i = 0; i < nr_secflavs; ++i) {
		READ_BUF(4);
		dummy = be32_to_cpup(p++);
		switch (dummy) {
		case RPC_AUTH_NULL:
			 
			if (cbs->flavor == (u32)(-1))
				cbs->flavor = RPC_AUTH_NULL;
			break;
		case RPC_AUTH_UNIX:
			READ_BUF(8);
			 
			dummy = be32_to_cpup(p++);

			 
			dummy = be32_to_cpup(p++);
			READ_BUF(dummy);
			SAVEMEM(machine_name, dummy);

			 
			READ_BUF(8);
			uid = be32_to_cpup(p++);
			gid = be32_to_cpup(p++);

			 
			READ_BUF(4);
			dummy = be32_to_cpup(p++);
			READ_BUF(dummy * 4);
			if (cbs->flavor == (u32)(-1)) {
				kuid_t kuid = make_kuid(&init_user_ns, uid);
				kgid_t kgid = make_kgid(&init_user_ns, gid);
				if (uid_valid(kuid) && gid_valid(kgid)) {
					cbs->uid = kuid;
					cbs->gid = kgid;
					cbs->flavor = RPC_AUTH_UNIX;
				} else {
					dprintk("RPC_AUTH_UNIX with invalid"
						"uid or gid ignoring!\n");
				}
			}
			break;
		case RPC_AUTH_GSS:
			dprintk("RPC_AUTH_GSS callback secflavor "
				"not supported!\n");
			READ_BUF(8);
			 
			dummy = be32_to_cpup(p++);
			 
			dummy = be32_to_cpup(p++);
			READ_BUF(dummy);
			p += XDR_QUADLEN(dummy);
			 
			READ_BUF(4);
			dummy = be32_to_cpup(p++);
			READ_BUF(dummy);
			break;
		default:
			dprintk("Illegal callback secflavor\n");
			return nfserr_inval;
		}
	}
	DECODE_TAIL;
}
