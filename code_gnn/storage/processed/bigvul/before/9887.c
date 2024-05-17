static void stream_int_shutw(struct stream_interface *si)
{
	si->ob->flags &= ~CF_SHUTW_NOW;
	if (si->ob->flags & CF_SHUTW)
		return;
	si->ob->flags |= CF_SHUTW;
	si->ob->wex = TICK_ETERNITY;
	si->flags &= ~SI_FL_WAIT_DATA;

	switch (si->state) {
	case SI_ST_EST:
		 
		if (!(si->flags & (SI_FL_ERR | SI_FL_NOLINGER)) &&
		    !(si->ib->flags & (CF_SHUTR|CF_DONT_READ)))
			return;

		 
	case SI_ST_CON:
	case SI_ST_CER:
	case SI_ST_QUE:
	case SI_ST_TAR:
		 
		si->state = SI_ST_DIS;
		si_applet_release(si);
	default:
		si->flags &= ~(SI_FL_WAIT_ROOM | SI_FL_NOLINGER);
		si->ib->flags &= ~CF_SHUTR_NOW;
		si->ib->flags |= CF_SHUTR;
		si->ib->rex = TICK_ETERNITY;
		si->exp = TICK_ETERNITY;
	}

	 
	if (!(si->flags & SI_FL_DONT_WAKE) && si->owner)
		task_wakeup(si->owner, TASK_WOKEN_IO);
}
