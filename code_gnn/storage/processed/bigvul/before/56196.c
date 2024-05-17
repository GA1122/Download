armpmu_reserve_hardware(struct arm_pmu *armpmu)
{
	int err, irq;
	unsigned int i, irqs;
	struct platform_device *pmu_device = armpmu->plat_device;

	if (!pmu_device) {
		pr_err("no PMU device registered\n");
		return -ENODEV;
	}

	irqs = min(pmu_device->num_resources, num_possible_cpus());
	if (!irqs) {
		pr_err("no irqs for PMUs defined\n");
		return -ENODEV;
	}

	irq = platform_get_irq(pmu_device, 0);
	if (irq <= 0) {
		pr_err("failed to get valid irq for PMU device\n");
		return -ENODEV;
	}

	if (irq_is_percpu(irq)) {
		err = request_percpu_irq(irq, armpmu->handle_irq,
				"arm-pmu", &cpu_hw_events);

		if (err) {
			pr_err("unable to request percpu IRQ%d for ARM PMU counters\n",
					irq);
			armpmu_release_hardware(armpmu);
			return err;
		}

		on_each_cpu(armpmu_enable_percpu_irq, &irq, 1);
	} else {
		for (i = 0; i < irqs; ++i) {
			err = 0;
			irq = platform_get_irq(pmu_device, i);
			if (irq <= 0)
				continue;

			 
			if (irq_set_affinity(irq, cpumask_of(i)) && irqs > 1) {
				pr_warning("unable to set irq affinity (irq=%d, cpu=%u)\n",
						irq, i);
				continue;
			}

			err = request_irq(irq, armpmu->handle_irq,
					IRQF_NOBALANCING,
					"arm-pmu", armpmu);
			if (err) {
				pr_err("unable to request IRQ%d for ARM PMU counters\n",
						irq);
				armpmu_release_hardware(armpmu);
				return err;
			}

			cpumask_set_cpu(i, &armpmu->active_irqs);
		}
	}

	return 0;
}