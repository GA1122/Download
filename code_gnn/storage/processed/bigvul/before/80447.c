GF_Err stdp_Size(GF_Box *s)
{
	GF_DegradationPriorityBox *ptr = (GF_DegradationPriorityBox *)s;

	ptr->size += (2 * ptr->nb_entries);
	return GF_OK;
}