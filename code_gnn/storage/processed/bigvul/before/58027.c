nft_rule_activate_next(struct net *net, struct nft_rule *rule)
{
	 
	rule->genmask = (1 << net->nft.gencursor);
}
