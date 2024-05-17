int platform_get_irq(struct platform_device *dev, unsigned int num)
{
#ifdef CONFIG_SPARC
	 
	if (!dev || num >= dev->archdata.num_irqs)
		return -ENXIO;
	return dev->archdata.irqs[num];
#else
	struct resource *r;
	if (IS_ENABLED(CONFIG_OF_IRQ) && dev->dev.of_node) {
		int ret;

		ret = of_irq_get(dev->dev.of_node, num);
		if (ret > 0 || ret == -EPROBE_DEFER)
			return ret;
	}

	r = platform_get_resource(dev, IORESOURCE_IRQ, num);
	if (has_acpi_companion(&dev->dev)) {
		if (r && r->flags & IORESOURCE_DISABLED) {
			int ret;

			ret = acpi_irq_get(ACPI_HANDLE(&dev->dev), num, r);
			if (ret)
				return ret;
		}
	}

	 
	if (r && r->flags & IORESOURCE_BITS) {
		struct irq_data *irqd;

		irqd = irq_get_irq_data(r->start);
		if (!irqd)
			return -ENXIO;
		irqd_set_trigger_type(irqd, r->flags & IORESOURCE_BITS);
	}

	return r ? r->start : -ENXIO;
#endif
}