bool kvm_irq_delivery_to_apic_fast(struct kvm *kvm, struct kvm_lapic *src,
		struct kvm_lapic_irq *irq, int *r, unsigned long *dest_map)
{
	struct kvm_apic_map *map;
	unsigned long bitmap = 1;
	struct kvm_lapic **dst;
	int i;
	bool ret = false;

	*r = -1;

	if (irq->shorthand == APIC_DEST_SELF) {
		*r = kvm_apic_set_irq(src->vcpu, irq, dest_map);
		return true;
	}

	if (irq->shorthand)
		return false;

	rcu_read_lock();
	map = rcu_dereference(kvm->arch.apic_map);

	if (!map)
		goto out;

	if (irq->dest_mode == 0) {  
		if (irq->delivery_mode == APIC_DM_LOWEST ||
				irq->dest_id == 0xff)
			goto out;
		dst = &map->phys_map[irq->dest_id & 0xff];
	} else {
		u32 mda = irq->dest_id << (32 - map->ldr_bits);

		dst = map->logical_map[apic_cluster_id(map, mda)];

		bitmap = apic_logical_id(map, mda);

		if (irq->delivery_mode == APIC_DM_LOWEST) {
			int l = -1;
			for_each_set_bit(i, &bitmap, 16) {
				if (!dst[i])
					continue;
				if (l < 0)
					l = i;
				else if (kvm_apic_compare_prio(dst[i]->vcpu, dst[l]->vcpu) < 0)
					l = i;
			}

			bitmap = (l >= 0) ? 1 << l : 0;
		}
	}

	for_each_set_bit(i, &bitmap, 16) {
		if (!dst[i])
			continue;
		if (*r < 0)
			*r = 0;
		*r += kvm_apic_set_irq(dst[i]->vcpu, irq, dest_map);
	}

	ret = true;
out:
	rcu_read_unlock();
	return ret;
}