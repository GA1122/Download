static int nfs4_handle_delegation_recall_error(struct nfs_server *server, struct nfs4_state *state, const nfs4_stateid *stateid, int err)
{
	switch (err) {
		default:
			printk(KERN_ERR "NFS: %s: unhandled error "
					"%d.\n", __func__, err);
		case 0:
		case -ENOENT:
		case -EAGAIN:
		case -ESTALE:
			break;
		case -NFS4ERR_BADSESSION:
		case -NFS4ERR_BADSLOT:
		case -NFS4ERR_BAD_HIGH_SLOT:
		case -NFS4ERR_CONN_NOT_BOUND_TO_SESSION:
		case -NFS4ERR_DEADSESSION:
			set_bit(NFS_DELEGATED_STATE, &state->flags);
			nfs4_schedule_session_recovery(server->nfs_client->cl_session, err);
			return -EAGAIN;
		case -NFS4ERR_STALE_CLIENTID:
		case -NFS4ERR_STALE_STATEID:
			set_bit(NFS_DELEGATED_STATE, &state->flags);
		case -NFS4ERR_EXPIRED:
			 
			nfs4_schedule_lease_recovery(server->nfs_client);
			return -EAGAIN;
		case -NFS4ERR_MOVED:
			nfs4_schedule_migration_recovery(server);
			return -EAGAIN;
		case -NFS4ERR_LEASE_MOVED:
			nfs4_schedule_lease_moved_recovery(server->nfs_client);
			return -EAGAIN;
		case -NFS4ERR_DELEG_REVOKED:
		case -NFS4ERR_ADMIN_REVOKED:
		case -NFS4ERR_BAD_STATEID:
		case -NFS4ERR_OPENMODE:
			nfs_inode_find_state_and_recover(state->inode,
					stateid);
			nfs4_schedule_stateid_recovery(server, state);
			return -EAGAIN;
		case -NFS4ERR_DELAY:
		case -NFS4ERR_GRACE:
			set_bit(NFS_DELEGATED_STATE, &state->flags);
			ssleep(1);
			return -EAGAIN;
		case -ENOMEM:
		case -NFS4ERR_DENIED:
			 
			return 0;
	}
	return err;
}