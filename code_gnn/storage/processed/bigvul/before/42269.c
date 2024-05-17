sg_get_rq_mark(Sg_fd * sfp, int pack_id)
{
	Sg_request *resp;
	unsigned long iflags;

	write_lock_irqsave(&sfp->rq_list_lock, iflags);
	for (resp = sfp->headrp; resp; resp = resp->nextrp) {
		 
		if ((1 == resp->done) && (!resp->sg_io_owned) &&
		    ((-1 == pack_id) || (resp->header.pack_id == pack_id))) {
			resp->done = 2;	 
			break;
		}
	}
	write_unlock_irqrestore(&sfp->rq_list_lock, iflags);
	return resp;
}
