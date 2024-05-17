 ppp_unregister_channel(struct ppp_channel *chan)
 {
 	struct channel *pch = chan->ppp;
 	struct ppp_net *pn;
 
 	if (!pch)
 		return;		 
 
 	chan->ppp = NULL;
 
 	 
 	down_write(&pch->chan_sem);
 	spin_lock_bh(&pch->downl);
 	pch->chan = NULL;
 	spin_unlock_bh(&pch->downl);
 	up_write(&pch->chan_sem);
 	ppp_disconnect_channel(pch);
 
 	pn = ppp_pernet(pch->chan_net);
  	spin_lock_bh(&pn->all_channels_lock);
  	list_del(&pch->list);
  	spin_unlock_bh(&pn->all_channels_lock);
// 	put_net(pch->chan_net);
// 	pch->chan_net = NULL;
  
  	pch->file.dead = 1;
  	wake_up_interruptible(&pch->file.rwait);
 	if (atomic_dec_and_test(&pch->file.refcnt))
 		ppp_destroy_channel(pch);
 }