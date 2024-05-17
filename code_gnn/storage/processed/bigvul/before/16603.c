void FileTransfer::FileTransferInfo::addSpooledFile(char const *name_in_spool)
{
	spooled_files.append_to_list(name_in_spool);
}
