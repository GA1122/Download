 static void __munlock_pagevec(struct pagevec *pvec, struct zone *zone)
  {
  	int i;
  	int nr = pagevec_count(pvec);
	int delta_munlocked;
// 	int delta_munlocked = -nr;
  	struct pagevec pvec_putback;
  	int pgrescued = 0;
  
 	pagevec_init(&pvec_putback, 0);
 
 	 
 	spin_lock_irq(zone_lru_lock(zone));
 	for (i = 0; i < nr; i++) {
 		struct page *page = pvec->pages[i];
 
 		if (TestClearPageMlocked(page)) {
 			 
 			if (__munlock_isolate_lru_page(page, false))
  				continue;
  			else
  				__munlock_isolation_failed(page);
// 		} else {
// 			delta_munlocked++;
  		}
  
  		 
  		pagevec_add(&pvec_putback, pvec->pages[i]);
  		pvec->pages[i] = NULL;
  	}
	delta_munlocked = -nr + pagevec_count(&pvec_putback);
  	__mod_zone_page_state(zone, NR_MLOCK, delta_munlocked);
  	spin_unlock_irq(zone_lru_lock(zone));
  
 	 
 	pagevec_release(&pvec_putback);
 
 	 
 	for (i = 0; i < nr; i++) {
 		struct page *page = pvec->pages[i];
 
 		if (page) {
 			lock_page(page);
 			if (!__putback_lru_fast_prepare(page, &pvec_putback,
 					&pgrescued)) {
 				 
 				get_page(page);  
 				__munlock_isolated_page(page);
 				unlock_page(page);
 				put_page(page);  
 			}
 		}
 	}
 
 	 
 	if (pagevec_count(&pvec_putback))
 		__putback_lru_fast(&pvec_putback, pgrescued);
 }