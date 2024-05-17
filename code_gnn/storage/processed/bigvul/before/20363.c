int kvm_io_bus_unregister_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			      struct kvm_io_device *dev)
{
	int i, r;
	struct kvm_io_bus *new_bus, *bus;

	bus = kvm->buses[bus_idx];

	new_bus = kmemdup(bus, sizeof(*bus), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;

	r = -ENOENT;
	for (i = 0; i < new_bus->dev_count; i++)
		if (new_bus->range[i].dev == dev) {
			r = 0;
			new_bus->dev_count--;
			new_bus->range[i] = new_bus->range[new_bus->dev_count];
			sort(new_bus->range, new_bus->dev_count,
			     sizeof(struct kvm_io_range),
			     kvm_io_bus_sort_cmp, NULL);
			break;
		}

	if (r) {
		kfree(new_bus);
		return r;
	}

	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);
	return r;
}
