vhost_scsi_make_nodeacl(struct se_portal_group *se_tpg,
		       struct config_group *group,
		       const char *name)
{
	struct se_node_acl *se_nacl, *se_nacl_new;
	struct vhost_scsi_nacl *nacl;
	u64 wwpn = 0;
	u32 nexus_depth;

	 
	se_nacl_new = vhost_scsi_alloc_fabric_acl(se_tpg);
	if (!se_nacl_new)
		return ERR_PTR(-ENOMEM);

	nexus_depth = 1;
	 
	se_nacl = core_tpg_add_initiator_node_acl(se_tpg, se_nacl_new,
				name, nexus_depth);
	if (IS_ERR(se_nacl)) {
		vhost_scsi_release_fabric_acl(se_tpg, se_nacl_new);
		return se_nacl;
	}
	 
	nacl = container_of(se_nacl, struct vhost_scsi_nacl, se_node_acl);
	nacl->iport_wwpn = wwpn;

	return se_nacl;
}
