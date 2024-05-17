struct sas_phy *sas_get_local_phy(struct domain_device *dev)
{
	struct sas_ha_struct *ha = dev->port->ha;
	struct sas_phy *phy;
	unsigned long flags;

	 
	BUG_ON(!dev->phy);

	spin_lock_irqsave(&ha->phy_port_lock, flags);
	phy = dev->phy;
	get_device(&phy->dev);
	spin_unlock_irqrestore(&ha->phy_port_lock, flags);

	return phy;
}
