rs_filter_set_previous(RSFilter *filter, RSFilter *previous)
{
	RS_DEBUG(FILTERS, "rs_filter_set_previous(%p, %p)", filter, previous);
	g_return_if_fail(RS_IS_FILTER(filter));
	g_return_if_fail(RS_IS_FILTER(previous));

	 
	if (filter->previous != previous)
	{
		if (filter->previous)
		{
			 
			filter->previous->next_filters = g_slist_remove(filter->previous->next_filters, filter);
			g_object_unref(filter->previous);
		}
		filter->previous = g_object_ref(previous);

		previous->next_filters = g_slist_append(previous->next_filters, filter);
	}
}