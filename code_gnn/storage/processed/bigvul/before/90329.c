megasas_enable_intr_gen2(struct megasas_instance *instance)
{
	struct megasas_register_set __iomem *regs;

	regs = instance->reg_set;
	writel(0xFFFFFFFF, &(regs)->outbound_doorbell_clear);

	 
	writel(~MFI_GEN2_ENABLE_INTERRUPT_MASK, &(regs)->outbound_intr_mask);

	 
	readl(&regs->outbound_intr_mask);
}
