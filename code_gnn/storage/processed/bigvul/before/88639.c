static irqreturn_t dwc3_check_event_buf(struct dwc3_event_buffer *evt)
{
	struct dwc3 *dwc = evt->dwc;
	u32 amount;
	u32 count;
	u32 reg;

	if (pm_runtime_suspended(dwc->dev)) {
		pm_runtime_get(dwc->dev);
		disable_irq_nosync(dwc->irq_gadget);
		dwc->pending_events = true;
		return IRQ_HANDLED;
	}

	 
	if (evt->flags & DWC3_EVENT_PENDING)
		return IRQ_HANDLED;

	count = dwc3_readl(dwc->regs, DWC3_GEVNTCOUNT(0));
	count &= DWC3_GEVNTCOUNT_MASK;
	if (!count)
		return IRQ_NONE;

	evt->count = count;
	evt->flags |= DWC3_EVENT_PENDING;

	 
	reg = dwc3_readl(dwc->regs, DWC3_GEVNTSIZ(0));
	reg |= DWC3_GEVNTSIZ_INTMASK;
	dwc3_writel(dwc->regs, DWC3_GEVNTSIZ(0), reg);

	amount = min(count, evt->length - evt->lpos);
	memcpy(evt->cache + evt->lpos, evt->buf + evt->lpos, amount);

	if (amount < count)
		memcpy(evt->cache, evt->buf, count - amount);

	dwc3_writel(dwc->regs, DWC3_GEVNTCOUNT(0), count);

	return IRQ_WAKE_THREAD;
}