void bond_set_mode_ops(struct bonding *bond, int mode)
{
	struct net_device *bond_dev = bond->dev;

	switch (mode) {
	case BOND_MODE_ROUNDROBIN:
		break;
	case BOND_MODE_ACTIVEBACKUP:
		break;
	case BOND_MODE_XOR:
		bond_set_xmit_hash_policy(bond);
		break;
	case BOND_MODE_BROADCAST:
		break;
	case BOND_MODE_8023AD:
		bond_set_xmit_hash_policy(bond);
		break;
	case BOND_MODE_ALB:
		 
	case BOND_MODE_TLB:
		break;
	default:
		 
		pr_err("%s: Error: Unknown bonding mode %d\n",
		       bond_dev->name, mode);
		break;
	}
}