int git_smart__download_pack(
	git_transport *transport,
	git_repository *repo,
	git_transfer_progress *stats,
	git_transfer_progress_cb transfer_progress_cb,
	void *progress_payload)
{
	transport_smart *t = (transport_smart *)transport;
	gitno_buffer *buf = &t->buffer;
	git_odb *odb;
	struct git_odb_writepack *writepack = NULL;
	int error = 0;
	struct network_packetsize_payload npp = {0};

	memset(stats, 0, sizeof(git_transfer_progress));

	if (transfer_progress_cb) {
		npp.callback = transfer_progress_cb;
		npp.payload = progress_payload;
		npp.stats = stats;
		t->packetsize_cb = &network_packetsize;
		t->packetsize_payload = &npp;

		 
		if (t->buffer.offset > 0 && !t->cancelled.val)
			if (t->packetsize_cb(t->buffer.offset, t->packetsize_payload))
				git_atomic_set(&t->cancelled, 1);
	}

	if ((error = git_repository_odb__weakptr(&odb, repo)) < 0 ||
		((error = git_odb_write_pack(&writepack, odb, transfer_progress_cb, progress_payload)) != 0))
		goto done;

	 
	if (!t->caps.side_band && !t->caps.side_band_64k) {
		error = no_sideband(t, writepack, buf, stats);
		goto done;
	}

	do {
		git_pkt *pkt = NULL;

		 
		if (t->cancelled.val) {
			giterr_clear();
			error = GIT_EUSER;
			goto done;
		}

		if ((error = recv_pkt(&pkt, buf)) >= 0) {
			 
			if (t->cancelled.val) {
				giterr_clear();
				error = GIT_EUSER;
			} else if (pkt->type == GIT_PKT_PROGRESS) {
				if (t->progress_cb) {
					git_pkt_progress *p = (git_pkt_progress *) pkt;
					error = t->progress_cb(p->data, p->len, t->message_cb_payload);
				}
			} else if (pkt->type == GIT_PKT_DATA) {
				git_pkt_data *p = (git_pkt_data *) pkt;

				if (p->len)
					error = writepack->append(writepack, p->data, p->len, stats);
			} else if (pkt->type == GIT_PKT_FLUSH) {
				 
				git__free(pkt);
				break;
			}
		}

		git__free(pkt);
		if (error < 0)
			goto done;

	} while (1);

	 
	if (npp.callback && npp.stats->received_bytes > npp.last_fired_bytes) {
		error = npp.callback(npp.stats, npp.payload);
		if (error != 0)
			goto done;
	}

	error = writepack->commit(writepack, stats);

done:
	if (writepack)
		writepack->free(writepack);
	if (transfer_progress_cb) {
		t->packetsize_cb = NULL;
		t->packetsize_payload = NULL;
	}

	return error;
}