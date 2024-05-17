static void mdiobus_release(struct device *d)
{
	struct mii_bus *bus = to_mii_bus(d);
	BUG_ON(bus->state != MDIOBUS_RELEASED &&
	        
	       bus->state != MDIOBUS_ALLOCATED);
	kfree(bus);
}
