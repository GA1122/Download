megasas_fire_cmd_skinny(struct megasas_instance *instance,
			dma_addr_t frame_phys_addr,
			u32 frame_count,
			struct megasas_register_set __iomem *regs)
{
	unsigned long flags;

	spin_lock_irqsave(&instance->hba_lock, flags);
	writel(upper_32_bits(frame_phys_addr),
	       &(regs)->inbound_high_queue_port);
	writel((lower_32_bits(frame_phys_addr) | (frame_count<<1))|1,
	       &(regs)->inbound_low_queue_port);
	mmiowb();
	spin_unlock_irqrestore(&instance->hba_lock, flags);
}