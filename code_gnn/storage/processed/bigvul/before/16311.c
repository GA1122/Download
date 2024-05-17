static const char * shadow_syscall_name(int condor_sysnum)
{
	switch(condor_sysnum) {
        case CONDOR_register_job_info: return "register_job_info";
        case CONDOR_register_starter_info: return "register_starter_info";
        case CONDOR_get_job_info: return "get_job_info";
        case CONDOR_get_user_info: return "get_user_info";
        case CONDOR_job_exit: return "job_exit";
        case CONDOR_job_termination: return "job_termination";
        case CONDOR_begin_execution: return "begin_execution";
        case CONDOR_open: return "open";
        case CONDOR_close: return "close";
        case CONDOR_read: return "read";
        case CONDOR_write: return "write";
        case CONDOR_lseek: return "lseek";
        case CONDOR_lseek64: return "lseek64";
        case CONDOR_llseek: return "llseek";
        case CONDOR_unlink: return "unlink";
        case CONDOR_rename: return "rename";
        case CONDOR_register_mpi_master_info: return "register_mpi_master_info";
        case CONDOR_mkdir: return "mkdir";
        case CONDOR_rmdir: return "rmdir";
        case CONDOR_fsync: return "fsync";
        case CONDOR_get_file_info_new: return "get_file_info_new";
        case CONDOR_ulog: return "ulog";
        case CONDOR_get_job_attr: return "get_job_attr";
        case CONDOR_set_job_attr: return "set_job_attr";
        case CONDOR_constrain: return "constrain";
        case CONDOR_get_sec_session_info: return "get_sec_session_info";
#ifdef WIN32
#else
        case CONDOR_pread: return "pread";
        case CONDOR_pwrite: return "pwrite";
        case CONDOR_sread: return "sread";
        case CONDOR_swrite: return "swrite";
        case CONDOR_rmall: return "rmall";
        case CONDOR_getfile: return "getfile";
        case CONDOR_putfile: return "putfile";
        case CONDOR_getlongdir: return "getlongdir";
        case CONDOR_getdir: return "getdir";
        case CONDOR_whoami: return "whoami";
        case CONDOR_whoareyou: return "whoareyou";
#endif
        case CONDOR_fstat: return "fstat";
        case CONDOR_fstatfs: return "fstatfs";
        case CONDOR_fchown: return "fchown";
        case CONDOR_fchmod: return "fchmod";
        case CONDOR_ftruncate: return "ftruncate";
        case CONDOR_link: return "link";
        case CONDOR_symlink: return "symlink";
        case CONDOR_readlink: return "readlink";
        case CONDOR_stat: return "stat";
        case CONDOR_lstat: return "lstat";
        case CONDOR_statfs: return "statfs";
        case CONDOR_access: return "access";
        case CONDOR_chmod: return "chmod";
		case CONDOR_chown: return "chown";
        case CONDOR_lchown: return "lchown";
        case CONDOR_truncate: return "truncate";
        case CONDOR_utime: return "utime";
	}
	return "unknown";
}
