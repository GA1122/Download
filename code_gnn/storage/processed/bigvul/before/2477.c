static size_t smbXcli_iov_len(const struct iovec *iov, int count)
{
	ssize_t ret = iov_buflen(iov, count);

	 
	return ret;
}