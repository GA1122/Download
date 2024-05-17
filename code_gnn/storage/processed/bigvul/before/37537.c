static int mmu_shrink(struct shrinker *shrink, struct shrink_control *sc)
{
	struct kvm *kvm;
	int nr_to_scan = sc->nr_to_scan;

	if (nr_to_scan == 0)
		goto out;

	raw_spin_lock(&kvm_lock);

	list_for_each_entry(kvm, &vm_list, vm_list) {
		int idx;
		LIST_HEAD(invalid_list);

		 
		if (!nr_to_scan--)
			break;
		 
		if (!kvm->arch.n_used_mmu_pages &&
		      !kvm_has_zapped_obsolete_pages(kvm))
			continue;

		idx = srcu_read_lock(&kvm->srcu);
		spin_lock(&kvm->mmu_lock);

		if (kvm_has_zapped_obsolete_pages(kvm)) {
			kvm_mmu_commit_zap_page(kvm,
			      &kvm->arch.zapped_obsolete_pages);
			goto unlock;
		}

		prepare_zap_oldest_mmu_page(kvm, &invalid_list);
		kvm_mmu_commit_zap_page(kvm, &invalid_list);

unlock:
		spin_unlock(&kvm->mmu_lock);
		srcu_read_unlock(&kvm->srcu, idx);

		list_move_tail(&kvm->vm_list, &vm_list);
		break;
	}

	raw_spin_unlock(&kvm_lock);

out:
	return percpu_counter_read_positive(&kvm_total_used_mmu_pages);
}
