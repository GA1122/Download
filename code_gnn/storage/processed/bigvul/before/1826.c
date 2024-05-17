gstate_to_update(fz_context *ctx, pdf_filter_processor *p)
{
	filter_gstate *gstate = p->gstate;

	 
	if (gstate->next != NULL)
		return gstate;

	 
	filter_push(ctx, p);
	gstate = p->gstate;
	gstate->pushed = 1;
	if (p->chain->op_q)
		p->chain->op_q(ctx, p->chain);

	return p->gstate;
}
