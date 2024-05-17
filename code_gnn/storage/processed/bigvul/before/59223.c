_kdc_is_anon_request(const KDC_REQ_BODY *b)
{
	 
	return (b->kdc_options.request_anonymous ||
		(b->kdc_options.constrained_delegation && !b->additional_tickets));
}
