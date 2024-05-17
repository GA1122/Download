void tcm_loop_drop_naa_tpg(
	struct se_portal_group *se_tpg)
{
	struct se_wwn *wwn = se_tpg->se_tpg_wwn;
	struct tcm_loop_tpg *tl_tpg = container_of(se_tpg,
				struct tcm_loop_tpg, tl_se_tpg);
	struct tcm_loop_hba *tl_hba;
	unsigned short tpgt;

	tl_hba = tl_tpg->tl_hba;
	tpgt = tl_tpg->tl_tpgt;
	 
	tcm_loop_drop_nexus(tl_tpg);
	 
	core_tpg_deregister(se_tpg);

	printk(KERN_INFO "TCM_Loop_ConfigFS: Deallocated Emulated %s"
		" Target Port %s,t,0x%04x\n", tcm_loop_dump_proto_id(tl_hba),
		config_item_name(&wwn->wwn_group.cg_item), tpgt);
}
