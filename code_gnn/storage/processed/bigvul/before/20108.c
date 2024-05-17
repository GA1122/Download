	__releases(proto_list_mutex)
{
	mutex_unlock(&proto_list_mutex);
}
