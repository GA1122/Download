smb_ofile_seek(
    smb_ofile_t		*of,
    ushort_t		mode,
    int32_t		off,
    uint32_t		*retoff)
{
	u_offset_t	newoff = 0;
	int		rc = 0;
	smb_attr_t	attr;

	ASSERT(of);
	ASSERT(of->f_magic == SMB_OFILE_MAGIC);
	ASSERT(of->f_refcnt);

	mutex_enter(&of->f_mutex);
	switch (mode) {
	case SMB_SEEK_SET:
		if (off < 0)
			newoff = 0;
		else
			newoff = (u_offset_t)off;
		break;

	case SMB_SEEK_CUR:
		if (off < 0 && (-off) > of->f_seek_pos)
			newoff = 0;
		else
			newoff = of->f_seek_pos + (u_offset_t)off;
		break;

	case SMB_SEEK_END:
		bzero(&attr, sizeof (smb_attr_t));
		attr.sa_mask |= SMB_AT_SIZE;
		rc = smb_fsop_getattr(NULL, zone_kcred(), of->f_node, &attr);
		if (rc != 0) {
			mutex_exit(&of->f_mutex);
			return (rc);
		}
		if (off < 0 && (-off) > attr.sa_vattr.va_size)
			newoff = 0;
		else
			newoff = attr.sa_vattr.va_size + (u_offset_t)off;
		break;

	default:
		mutex_exit(&of->f_mutex);
		return (EINVAL);
	}

	 

	if (newoff > UINT_MAX) {
		rc = EOVERFLOW;
	} else {
		of->f_seek_pos = newoff;
		*retoff = (uint32_t)newoff;
	}
	mutex_exit(&of->f_mutex);
 	return (rc);
 }