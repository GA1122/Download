 static void balloon_process(struct work_struct *work)
 {
 	enum bp_state state = BP_DONE;
 	long credit;
 
 
 	do {
 		mutex_lock(&balloon_mutex);
 
 		credit = current_credit();
 
 		if (credit > 0) {
 			if (balloon_is_inflated())
 				state = increase_reservation(credit);
 			else
  				state = reserve_additional_memory();
  		}
  
		if (credit < 0)
			state = decrease_reservation(-credit, GFP_BALLOON);
// 		if (credit < 0) {
// 			long n_pages;
// 
// 			n_pages = min(-credit, si_mem_available());
// 			state = decrease_reservation(n_pages, GFP_BALLOON);
// 			if (state == BP_DONE && n_pages != -credit &&
// 			    n_pages < totalreserve_pages)
// 				state = BP_EAGAIN;
// 		}
  
  		state = update_schedule(state);
  
 		mutex_unlock(&balloon_mutex);
 
 		cond_resched();
 
 	} while (credit && state == BP_DONE);
 
 	 
 	if (state == BP_EAGAIN)
 		schedule_delayed_work(&balloon_worker, balloon_stats.schedule_delay * HZ);
 }