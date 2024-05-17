void file_bcast_init(void)
{
	 
	file_bcast_list = list_create((ListDelF) _free_file_bcast_info_t);
}
