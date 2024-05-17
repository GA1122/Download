static irqreturn_t atl2_intr(int irq, void *data)
{
	struct atl2_adapter *adapter = netdev_priv(data);
	struct atl2_hw *hw = &adapter->hw;
	u32 status;

	status = ATL2_READ_REG(hw, REG_ISR);
	if (0 == status)
		return IRQ_NONE;

	 
	if (status & ISR_PHY)
		atl2_clear_phy_int(adapter);

	 
	ATL2_WRITE_REG(hw, REG_ISR, status | ISR_DIS_INT);

	 
	if (status & ISR_PHY_LINKDOWN) {
		if (netif_running(adapter->netdev)) {  
			ATL2_WRITE_REG(hw, REG_ISR, 0);
			ATL2_WRITE_REG(hw, REG_IMR, 0);
			ATL2_WRITE_FLUSH(hw);
			schedule_work(&adapter->reset_task);
			return IRQ_HANDLED;
		}
	}

	 
	if (status & (ISR_DMAR_TO_RST | ISR_DMAW_TO_RST)) {
		ATL2_WRITE_REG(hw, REG_ISR, 0);
		ATL2_WRITE_REG(hw, REG_IMR, 0);
		ATL2_WRITE_FLUSH(hw);
		schedule_work(&adapter->reset_task);
		return IRQ_HANDLED;
	}

	 
	if (status & (ISR_PHY | ISR_MANUAL)) {
		adapter->netdev->stats.tx_carrier_errors++;
		atl2_check_for_link(adapter);
	}

	 
	if (status & ISR_TX_EVENT)
		atl2_intr_tx(adapter);

	 
	if (status & ISR_RX_EVENT)
		atl2_intr_rx(adapter);

	 
	ATL2_WRITE_REG(&adapter->hw, REG_ISR, 0);
	return IRQ_HANDLED;
}
