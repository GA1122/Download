void sock_update_memcg(struct sock *sk)
{
	if (mem_cgroup_sockets_enabled) {
		struct mem_cgroup *memcg;

		BUG_ON(!sk->sk_prot->proto_cgroup);

		 
		if (sk->sk_cgrp) {
			BUG_ON(mem_cgroup_is_root(sk->sk_cgrp->memcg));
			mem_cgroup_get(sk->sk_cgrp->memcg);
			return;
		}

		rcu_read_lock();
		memcg = mem_cgroup_from_task(current);
		if (!mem_cgroup_is_root(memcg)) {
			mem_cgroup_get(memcg);
			sk->sk_cgrp = sk->sk_prot->proto_cgroup(memcg);
		}
		rcu_read_unlock();
	}
}
