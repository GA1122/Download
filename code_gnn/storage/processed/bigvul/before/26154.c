void perf_prepare_sample(struct perf_event_header *header,
			 struct perf_sample_data *data,
			 struct perf_event *event,
			 struct pt_regs *regs)
{
	u64 sample_type = event->attr.sample_type;

	header->type = PERF_RECORD_SAMPLE;
	header->size = sizeof(*header) + event->header_size;

	header->misc = 0;
	header->misc |= perf_misc_flags(regs);

	__perf_event_header__init_id(header, data, event);

	if (sample_type & PERF_SAMPLE_IP)
		data->ip = perf_instruction_pointer(regs);

	if (sample_type & PERF_SAMPLE_CALLCHAIN) {
		int size = 1;

		data->callchain = perf_callchain(regs);

		if (data->callchain)
			size += data->callchain->nr;

		header->size += size * sizeof(u64);
	}

	if (sample_type & PERF_SAMPLE_RAW) {
		int size = sizeof(u32);

		if (data->raw)
			size += data->raw->size;
		else
			size += sizeof(u32);

		WARN_ON_ONCE(size & (sizeof(u64)-1));
		header->size += size;
 	}
 }