static inline u8 l2cap_get_ident(struct l2cap_conn *conn)
{
	u8 id;

	 

	spin_lock_bh(&conn->lock);

	if (++conn->tx_ident > 128)
		conn->tx_ident = 1;

	id = conn->tx_ident;

	spin_unlock_bh(&conn->lock);

	return id;
}
