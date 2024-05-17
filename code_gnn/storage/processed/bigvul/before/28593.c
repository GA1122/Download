int qeth_mdio_read(struct net_device *dev, int phy_id, int regnum)
{
	struct qeth_card *card = dev->ml_priv;
	int rc = 0;

	switch (regnum) {
	case MII_BMCR:  
		rc = BMCR_FULLDPLX;
		if ((card->info.link_type != QETH_LINK_TYPE_GBIT_ETH) &&
		    (card->info.link_type != QETH_LINK_TYPE_OSN) &&
		    (card->info.link_type != QETH_LINK_TYPE_10GBIT_ETH))
			rc |= BMCR_SPEED100;
		break;
	case MII_BMSR:  
		rc = BMSR_ERCAP | BMSR_ANEGCOMPLETE | BMSR_LSTATUS |
		     BMSR_10HALF | BMSR_10FULL | BMSR_100HALF | BMSR_100FULL |
		     BMSR_100BASE4;
		break;
	case MII_PHYSID1:  
		rc = (dev->dev_addr[0] << 16) | (dev->dev_addr[1] << 8) |
		     dev->dev_addr[2];
		rc = (rc >> 5) & 0xFFFF;
		break;
	case MII_PHYSID2:  
		rc = (dev->dev_addr[2] << 10) & 0xFFFF;
		break;
	case MII_ADVERTISE:  
		rc = ADVERTISE_ALL;
		break;
	case MII_LPA:  
		rc = LPA_10HALF | LPA_10FULL | LPA_100HALF | LPA_100FULL |
		     LPA_100BASE4 | LPA_LPACK;
		break;
	case MII_EXPANSION:  
		break;
	case MII_DCOUNTER:  
		break;
	case MII_FCSCOUNTER:  
		break;
	case MII_NWAYTEST:  
		break;
	case MII_RERRCOUNTER:  
		rc = card->stats.rx_errors;
		break;
	case MII_SREVISION:  
		break;
	case MII_RESV1:  
		break;
	case MII_LBRERROR:  
		break;
	case MII_PHYADDR:  
		break;
	case MII_RESV2:  
		break;
	case MII_TPISTATUS:  
		break;
	case MII_NCONFIG:  
		break;
	default:
		break;
	}
	return rc;
}
