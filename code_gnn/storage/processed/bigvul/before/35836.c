void sctp_chunk_assign_tsn(struct sctp_chunk *chunk)
{
	if (!chunk->has_tsn) {
		 
		chunk->subh.data_hdr->tsn =
			htonl(sctp_association_get_next_tsn(chunk->asoc));
		chunk->has_tsn = 1;
	}
}