static void vcpu_global_purge(void *info)
{
	struct call_data *p = (struct call_data *)info;
	struct kvm_vcpu *vcpu = p->vcpu;

	if (test_bit(KVM_REQ_TLB_FLUSH, &vcpu->requests))
		return;

	set_bit(KVM_REQ_PTC_G, &vcpu->requests);
	if (vcpu->arch.ptc_g_count < MAX_PTC_G_NUM) {
		vcpu->arch.ptc_g_data[vcpu->arch.ptc_g_count++] =
							p->ptc_g_data;
	} else {
		clear_bit(KVM_REQ_PTC_G, &vcpu->requests);
		vcpu->arch.ptc_g_count = 0;
		set_bit(KVM_REQ_TLB_FLUSH, &vcpu->requests);
	}
}
