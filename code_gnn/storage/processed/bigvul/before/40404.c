static void caif_ctrl_cb(struct cflayer *layr,
			 enum caif_ctrlcmd flow,
			 int phyid)
{
	struct caifsock *cf_sk = container_of(layr, struct caifsock, layer);
	switch (flow) {
	case CAIF_CTRLCMD_FLOW_ON_IND:
		 
		set_tx_flow_on(cf_sk);
		cf_sk->sk.sk_state_change(&cf_sk->sk);
		break;

	case CAIF_CTRLCMD_FLOW_OFF_IND:
		 
		set_tx_flow_off(cf_sk);
		cf_sk->sk.sk_state_change(&cf_sk->sk);
		break;

	case CAIF_CTRLCMD_INIT_RSP:
		 
		caif_client_register_refcnt(&cf_sk->layer,
						cfsk_hold, cfsk_put);
		cf_sk->sk.sk_state = CAIF_CONNECTED;
		set_tx_flow_on(cf_sk);
		cf_sk->sk.sk_shutdown = 0;
		cf_sk->sk.sk_state_change(&cf_sk->sk);
		break;

	case CAIF_CTRLCMD_DEINIT_RSP:
		 
		cf_sk->sk.sk_state = CAIF_DISCONNECTED;
		cf_sk->sk.sk_state_change(&cf_sk->sk);
		break;

	case CAIF_CTRLCMD_INIT_FAIL_RSP:
		 
		cf_sk->sk.sk_err = ECONNREFUSED;
		cf_sk->sk.sk_state = CAIF_DISCONNECTED;
		cf_sk->sk.sk_shutdown = SHUTDOWN_MASK;
		 
		set_tx_flow_on(cf_sk);
		cf_sk->sk.sk_state_change(&cf_sk->sk);
		break;

	case CAIF_CTRLCMD_REMOTE_SHUTDOWN_IND:
		 
		cf_sk->sk.sk_shutdown = SHUTDOWN_MASK;
		cf_sk->sk.sk_err = ECONNRESET;
		set_rx_flow_on(cf_sk);
		cf_sk->sk.sk_error_report(&cf_sk->sk);
		break;

	default:
		pr_debug("Unexpected flow command %d\n", flow);
	}
}
