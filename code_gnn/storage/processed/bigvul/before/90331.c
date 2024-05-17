megasas_enable_intr_xscale(struct megasas_instance *instance)
{
	struct megasas_register_set __iomem *regs;

	regs = instance->reg_set;
	writel(0, &(regs)->outbound_intr_mask);

	 
	readl(&regs->outbound_intr_mask);
}
