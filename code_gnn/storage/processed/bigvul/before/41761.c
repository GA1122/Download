static void addrconf_dad_work(struct work_struct *w)
{
	struct inet6_ifaddr *ifp = container_of(to_delayed_work(w),
						struct inet6_ifaddr,
						dad_work);
	struct inet6_dev *idev = ifp->idev;
	struct in6_addr mcaddr;

	enum {
		DAD_PROCESS,
		DAD_BEGIN,
		DAD_ABORT,
	} action = DAD_PROCESS;

	rtnl_lock();

	spin_lock_bh(&ifp->state_lock);
	if (ifp->state == INET6_IFADDR_STATE_PREDAD) {
		action = DAD_BEGIN;
		ifp->state = INET6_IFADDR_STATE_DAD;
	} else if (ifp->state == INET6_IFADDR_STATE_ERRDAD) {
		action = DAD_ABORT;
		ifp->state = INET6_IFADDR_STATE_POSTDAD;
	}
	spin_unlock_bh(&ifp->state_lock);

	if (action == DAD_BEGIN) {
		addrconf_dad_begin(ifp);
		goto out;
	} else if (action == DAD_ABORT) {
		addrconf_dad_stop(ifp, 1);
		goto out;
	}

	if (!ifp->dad_probes && addrconf_dad_end(ifp))
		goto out;

	write_lock_bh(&idev->lock);
	if (idev->dead || !(idev->if_flags & IF_READY)) {
		write_unlock_bh(&idev->lock);
		goto out;
	}

	spin_lock(&ifp->lock);
	if (ifp->state == INET6_IFADDR_STATE_DEAD) {
		spin_unlock(&ifp->lock);
		write_unlock_bh(&idev->lock);
		goto out;
	}

	if (ifp->dad_probes == 0) {
		 

		ifp->flags &= ~(IFA_F_TENTATIVE|IFA_F_OPTIMISTIC|IFA_F_DADFAILED);
		spin_unlock(&ifp->lock);
		write_unlock_bh(&idev->lock);

		addrconf_dad_completed(ifp);

		goto out;
	}

	ifp->dad_probes--;
	addrconf_mod_dad_work(ifp,
			      NEIGH_VAR(ifp->idev->nd_parms, RETRANS_TIME));
	spin_unlock(&ifp->lock);
	write_unlock_bh(&idev->lock);

	 
	addrconf_addr_solict_mult(&ifp->addr, &mcaddr);
	ndisc_send_ns(ifp->idev->dev, NULL, &ifp->addr, &mcaddr, &in6addr_any);
out:
	in6_ifa_put(ifp);
	rtnl_unlock();
}