int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, struct kvm_io_device *dev)
{
	struct kvm_io_bus *new_bus, *bus;

	bus = kvm->buses[bus_idx];
	if (bus->dev_count > NR_IOBUS_DEVS-1)
		return -ENOSPC;

	new_bus = kmemdup(bus, sizeof(struct kvm_io_bus), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;
	kvm_io_bus_insert_dev(new_bus, dev, addr, len);
	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);

	return 0;
}