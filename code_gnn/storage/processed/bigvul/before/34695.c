static loff_t gfs2_llseek(struct file *file, loff_t offset, int origin)
{
	struct gfs2_inode *ip = GFS2_I(file->f_mapping->host);
	struct gfs2_holder i_gh;
	loff_t error;

	switch (origin) {
	case SEEK_END:  
	case SEEK_DATA:
	case SEEK_HOLE:
		error = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED, LM_FLAG_ANY,
					   &i_gh);
		if (!error) {
			error = generic_file_llseek_unlocked(file, offset, origin);
			gfs2_glock_dq_uninit(&i_gh);
		}
		break;
	case SEEK_CUR:
	case SEEK_SET:
		error = generic_file_llseek_unlocked(file, offset, origin);
		break;
	default:
		error = -EINVAL;
	}

	return error;
}