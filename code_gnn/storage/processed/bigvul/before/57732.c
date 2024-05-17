static int kvm_guest_time_update(struct kvm_vcpu *v)
{
	unsigned long flags, this_tsc_khz, tgt_tsc_khz;
	struct kvm_vcpu_arch *vcpu = &v->arch;
	struct kvm_arch *ka = &v->kvm->arch;
	s64 kernel_ns;
	u64 tsc_timestamp, host_tsc;
	struct pvclock_vcpu_time_info guest_hv_clock;
	u8 pvclock_flags;
	bool use_master_clock;

	kernel_ns = 0;
	host_tsc = 0;

	 
	spin_lock(&ka->pvclock_gtod_sync_lock);
	use_master_clock = ka->use_master_clock;
	if (use_master_clock) {
		host_tsc = ka->master_cycle_now;
		kernel_ns = ka->master_kernel_ns;
	}
	spin_unlock(&ka->pvclock_gtod_sync_lock);

	 
	local_irq_save(flags);
	this_tsc_khz = __this_cpu_read(cpu_tsc_khz);
	if (unlikely(this_tsc_khz == 0)) {
		local_irq_restore(flags);
		kvm_make_request(KVM_REQ_CLOCK_UPDATE, v);
		return 1;
	}
	if (!use_master_clock) {
		host_tsc = rdtsc();
		kernel_ns = get_kernel_ns();
	}

	tsc_timestamp = kvm_read_l1_tsc(v, host_tsc);

	 
	if (vcpu->tsc_catchup) {
		u64 tsc = compute_guest_tsc(v, kernel_ns);
		if (tsc > tsc_timestamp) {
			adjust_tsc_offset_guest(v, tsc - tsc_timestamp);
			tsc_timestamp = tsc;
		}
	}

	local_irq_restore(flags);

	if (!vcpu->pv_time_enabled)
		return 0;

	if (unlikely(vcpu->hw_tsc_khz != this_tsc_khz)) {
		tgt_tsc_khz = kvm_has_tsc_control ?
			vcpu->virtual_tsc_khz : this_tsc_khz;
		kvm_get_time_scale(NSEC_PER_SEC / 1000, tgt_tsc_khz,
				   &vcpu->hv_clock.tsc_shift,
				   &vcpu->hv_clock.tsc_to_system_mul);
		vcpu->hw_tsc_khz = this_tsc_khz;
	}

	 
	vcpu->hv_clock.tsc_timestamp = tsc_timestamp;
	vcpu->hv_clock.system_time = kernel_ns + v->kvm->arch.kvmclock_offset;
	vcpu->last_guest_tsc = tsc_timestamp;

	if (unlikely(kvm_read_guest_cached(v->kvm, &vcpu->pv_time,
		&guest_hv_clock, sizeof(guest_hv_clock))))
		return 0;

	 
	BUILD_BUG_ON(offsetof(struct pvclock_vcpu_time_info, version) != 0);

	vcpu->hv_clock.version = guest_hv_clock.version + 1;
	kvm_write_guest_cached(v->kvm, &vcpu->pv_time,
				&vcpu->hv_clock,
				sizeof(vcpu->hv_clock.version));

	smp_wmb();

	 
	pvclock_flags = (guest_hv_clock.flags & PVCLOCK_GUEST_STOPPED);

	if (vcpu->pvclock_set_guest_stopped_request) {
		pvclock_flags |= PVCLOCK_GUEST_STOPPED;
		vcpu->pvclock_set_guest_stopped_request = false;
	}

	 
	if (use_master_clock)
		pvclock_flags |= PVCLOCK_TSC_STABLE_BIT;

	vcpu->hv_clock.flags = pvclock_flags;

	trace_kvm_pvclock_update(v->vcpu_id, &vcpu->hv_clock);

	kvm_write_guest_cached(v->kvm, &vcpu->pv_time,
				&vcpu->hv_clock,
				sizeof(vcpu->hv_clock));

	smp_wmb();

	vcpu->hv_clock.version++;
	kvm_write_guest_cached(v->kvm, &vcpu->pv_time,
				&vcpu->hv_clock,
				sizeof(vcpu->hv_clock.version));
	return 0;
}