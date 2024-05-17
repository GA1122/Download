 static int do_timer_create(clockid_t which_clock, struct sigevent *event,
 			   timer_t __user *created_timer_id)
 {
 	const struct k_clock *kc = clockid_to_kclock(which_clock);
 	struct k_itimer *new_timer;
 	int error, new_timer_id;
 	int it_id_set = IT_ID_NOT_SET;
 
 	if (!kc)
 		return -EINVAL;
 	if (!kc->timer_create)
 		return -EOPNOTSUPP;
 
 	new_timer = alloc_posix_timer();
 	if (unlikely(!new_timer))
 		return -EAGAIN;
 
 	spin_lock_init(&new_timer->it_lock);
 	new_timer_id = posix_timer_add(new_timer);
 	if (new_timer_id < 0) {
 		error = new_timer_id;
 		goto out;
 	}
 
 	it_id_set = IT_ID_SET;
  	new_timer->it_id = (timer_t) new_timer_id;
  	new_timer->it_clock = which_clock;
  	new_timer->kclock = kc;
	new_timer->it_overrun = -1;
// 	new_timer->it_overrun = -1LL;
  
  	if (event) {
  		rcu_read_lock();
 		new_timer->it_pid = get_pid(good_sigevent(event));
 		rcu_read_unlock();
 		if (!new_timer->it_pid) {
 			error = -EINVAL;
 			goto out;
 		}
 		new_timer->it_sigev_notify     = event->sigev_notify;
 		new_timer->sigq->info.si_signo = event->sigev_signo;
 		new_timer->sigq->info.si_value = event->sigev_value;
 	} else {
 		new_timer->it_sigev_notify     = SIGEV_SIGNAL;
 		new_timer->sigq->info.si_signo = SIGALRM;
 		memset(&new_timer->sigq->info.si_value, 0, sizeof(sigval_t));
 		new_timer->sigq->info.si_value.sival_int = new_timer->it_id;
 		new_timer->it_pid = get_pid(task_tgid(current));
 	}
 
 	new_timer->sigq->info.si_tid   = new_timer->it_id;
 	new_timer->sigq->info.si_code  = SI_TIMER;
 
 	if (copy_to_user(created_timer_id,
 			 &new_timer_id, sizeof (new_timer_id))) {
 		error = -EFAULT;
 		goto out;
 	}
 
 	error = kc->timer_create(new_timer);
 	if (error)
 		goto out;
 
 	spin_lock_irq(&current->sighand->siglock);
 	new_timer->it_signal = current->signal;
 	list_add(&new_timer->list, &current->signal->posix_timers);
 	spin_unlock_irq(&current->sighand->siglock);
 
 	return 0;
 	 
 out:
 	release_posix_timer(new_timer, it_id_set);
 	return error;
 }