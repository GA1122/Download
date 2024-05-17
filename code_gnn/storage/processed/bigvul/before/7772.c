static int h2_process_mux(struct h2c *h2c)
{
	struct h2s *h2s, *h2s_back;

	 
	if (h2c->rcvd_c > 0 &&
	    !(h2c->flags & (H2_CF_MUX_MFULL | H2_CF_MUX_MALLOC)) &&
	    h2c_send_conn_wu(h2c) < 0)
		goto fail;

	 

	list_for_each_entry_safe(h2s, h2s_back, &h2c->fctl_list, list) {
		if (h2c->mws <= 0 || h2c->flags & H2_CF_MUX_BLOCK_ANY ||
		    h2c->st0 >= H2_CS_ERROR)
			break;

		 
		h2s->flags &= ~H2_SF_BLK_ANY;

		if (h2s->cs) {
			h2s->cs->data_cb->send(h2s->cs);
			h2s->cs->data_cb->wake(h2s->cs);
		} else {
			h2s_send_rst_stream(h2c, h2s);
		}

		 
		if ((h2s->flags & H2_SF_BLK_MFCTL) == 0) {
			if (h2s->flags & H2_SF_BLK_ANY) {
				LIST_DEL(&h2s->list);
				LIST_ADDQ(&h2c->send_list, &h2s->list);
			}
			else {
				LIST_DEL(&h2s->list);
				LIST_INIT(&h2s->list);
				if (h2s->cs)
					h2s->cs->flags &= ~CS_FL_DATA_WR_ENA;
				else {
					 
					h2s_destroy(h2s);
				}
			}
		}
	}

	list_for_each_entry_safe(h2s, h2s_back, &h2c->send_list, list) {
		if (h2c->st0 >= H2_CS_ERROR || h2c->flags & H2_CF_MUX_BLOCK_ANY)
			break;

		 
		h2s->flags &= ~H2_SF_BLK_ANY;

		if (h2s->cs) {
			h2s->cs->data_cb->send(h2s->cs);
			h2s->cs->data_cb->wake(h2s->cs);
		} else {
			h2s_send_rst_stream(h2c, h2s);
		}
		 
		if (h2s->flags & H2_SF_BLK_MFCTL) {
			 
			LIST_DEL(&h2s->list);
			LIST_ADDQ(&h2c->fctl_list, &h2s->list);
		}
		else if (!(h2s->flags & H2_SF_BLK_ANY)) {
			LIST_DEL(&h2s->list);
			LIST_INIT(&h2s->list);
			if (h2s->cs)
				h2s->cs->flags &= ~CS_FL_DATA_WR_ENA;
			else {
				 
				h2s_destroy(h2s);
			}
		}
	}

 fail:
	if (unlikely(h2c->st0 >= H2_CS_ERROR)) {
		if (h2c->st0 == H2_CS_ERROR) {
			if (h2c->max_id >= 0) {
				h2c_send_goaway_error(h2c, NULL);
				if (h2c->flags & H2_CF_MUX_BLOCK_ANY)
					return 0;
			}

			h2c->st0 = H2_CS_ERROR2;  
		}
		return 1;
	}
	return (h2c->mws <= 0 || LIST_ISEMPTY(&h2c->fctl_list)) && LIST_ISEMPTY(&h2c->send_list);
}
