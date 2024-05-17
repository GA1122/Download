static int parse_sid(struct cifs_sid *psid, char *end_of_acl)
{
	 

	 
	if (end_of_acl < (char *)psid + 8) {
		cifs_dbg(VFS, "ACL too small to parse SID %p\n", psid);
		return -EINVAL;
	}

#ifdef CONFIG_CIFS_DEBUG2
	if (psid->num_subauth) {
		int i;
		cifs_dbg(FYI, "SID revision %d num_auth %d\n",
			 psid->revision, psid->num_subauth);

		for (i = 0; i < psid->num_subauth; i++) {
			cifs_dbg(FYI, "SID sub_auth[%d]: 0x%x\n",
				 i, le32_to_cpu(psid->sub_auth[i]));
		}

		 
		cifs_dbg(FYI, "RID 0x%x\n",
			 le32_to_cpu(psid->sub_auth[psid->num_subauth-1]));
	}
#endif

	return 0;
}
