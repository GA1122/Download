static inline void l2cap_sig_channel(struct l2cap_conn *conn, struct sk_buff *skb)
{
	u8 *data = skb->data;
	int len = skb->len;
	struct l2cap_cmd_hdr cmd;
	int err = 0;

	l2cap_raw_recv(conn, skb);

	while (len >= L2CAP_CMD_HDR_SIZE) {
		u16 cmd_len;
		memcpy(&cmd, data, L2CAP_CMD_HDR_SIZE);
		data += L2CAP_CMD_HDR_SIZE;
		len  -= L2CAP_CMD_HDR_SIZE;

		cmd_len = le16_to_cpu(cmd.len);

		BT_DBG("code 0x%2.2x len %d id 0x%2.2x", cmd.code, cmd_len, cmd.ident);

		if (cmd_len > len || !cmd.ident) {
			BT_DBG("corrupted command");
			break;
		}

		switch (cmd.code) {
		case L2CAP_COMMAND_REJ:
			l2cap_command_rej(conn, &cmd, data);
			break;

		case L2CAP_CONN_REQ:
			err = l2cap_connect_req(conn, &cmd, data);
			break;

		case L2CAP_CONN_RSP:
			err = l2cap_connect_rsp(conn, &cmd, data);
			break;

		case L2CAP_CONF_REQ:
			err = l2cap_config_req(conn, &cmd, cmd_len, data);
			break;

		case L2CAP_CONF_RSP:
			err = l2cap_config_rsp(conn, &cmd, data);
			break;

		case L2CAP_DISCONN_REQ:
			err = l2cap_disconnect_req(conn, &cmd, data);
			break;

		case L2CAP_DISCONN_RSP:
			err = l2cap_disconnect_rsp(conn, &cmd, data);
			break;

		case L2CAP_ECHO_REQ:
			l2cap_send_cmd(conn, cmd.ident, L2CAP_ECHO_RSP, cmd_len, data);
			break;

		case L2CAP_ECHO_RSP:
			break;

		case L2CAP_INFO_REQ:
			err = l2cap_information_req(conn, &cmd, data);
			break;

		case L2CAP_INFO_RSP:
			err = l2cap_information_rsp(conn, &cmd, data);
			break;

		default:
			BT_ERR("Unknown signaling command 0x%2.2x", cmd.code);
			err = -EINVAL;
			break;
		}

		if (err) {
			struct l2cap_cmd_rej rej;
			BT_DBG("error %d", err);

			 
			rej.reason = cpu_to_le16(0);
			l2cap_send_cmd(conn, cmd.ident, L2CAP_COMMAND_REJ, sizeof(rej), &rej);
		}

		data += cmd_len;
		len  -= cmd_len;
	}

	kfree_skb(skb);
}