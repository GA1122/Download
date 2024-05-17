static irqreturn_t irq_handler(int intno, void *dev_id)
{
	int irq_bit;
	unsigned long flags;

	spin_lock_irqsave(&irqbits_lock, flags);
	irq_bit = 1 << intno;
	if ((irqbits & irq_bit) || !vm86_irqs[intno].tsk)
		goto out;
	irqbits |= irq_bit;
	if (vm86_irqs[intno].sig)
		send_sig(vm86_irqs[intno].sig, vm86_irqs[intno].tsk, 1);
	 
	disable_irq_nosync(intno);
	spin_unlock_irqrestore(&irqbits_lock, flags);
	return IRQ_HANDLED;

out:
	spin_unlock_irqrestore(&irqbits_lock, flags);
	return IRQ_NONE;
}