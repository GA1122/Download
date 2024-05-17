SchedulerObject::submit(AttributeMapType &jobAdMap, std::string &id, std::string &text)
{
	int cluster;
	int proc;

    if (!m_codec) {
        text = "Codec has not been initialized";
        return false;
    }

	const char* required[] = {
				ATTR_JOB_CMD,
				ATTR_REQUIREMENTS,
				ATTR_OWNER,
				ATTR_JOB_IWD,
				NULL
				};

	BeginTransaction();

	if (-1 == (cluster = NewCluster())) {
		AbortTransaction();
		text = "Failed to create new cluster";
		return false;
	}

	if (-1 == (proc = NewProc(cluster))) {
		AbortTransaction();
		text = "Failed to create new proc";
		return false;
	}





	ClassAd ad;
	int universe;

    ad.Assign(ATTR_SHOULD_TRANSFER_FILES, "NO");

	if (!m_codec->mapToClassAd(jobAdMap, ad, text)) {
		AbortTransaction();
		return false;
	}

	std::string missing;
	if (!checkRequiredAttrs(ad, required, missing)) {
		AbortTransaction();
		text = "Job ad is missing required attributes: " + missing;
		return false;
	}

	::SetAttribute(cluster, proc, ATTR_JOB_STATUS, "1");  

	::SetAttribute(cluster, proc, ATTR_JOB_REMOTE_USER_CPU, "0.0");  
	::SetAttribute(cluster, proc, ATTR_JOB_PRIO, "0");               
	::SetAttribute(cluster, proc, ATTR_IMAGE_SIZE, "0");             

	if (!ad.LookupInteger(ATTR_JOB_UNIVERSE, universe)) {
		char* uni_str = param("DEFAULT_UNIVERSE");
		if (!uni_str) {
			universe = CONDOR_UNIVERSE_VANILLA;
		}
		else {
			universe = CondorUniverseNumber(uni_str);
		}
		::SetAttributeInt(cluster, proc, ATTR_JOB_UNIVERSE, universe );
	}
	if ( universe != CONDOR_UNIVERSE_MPI && universe != CONDOR_UNIVERSE_PVM ) {
		::SetAttribute(cluster, proc, ATTR_MAX_HOSTS, "1");               
		::SetAttribute(cluster, proc, ATTR_MIN_HOSTS, "1");             
	}
	::SetAttribute(cluster, proc, ATTR_CURRENT_HOSTS, "0");  

	ExprTree *expr;
	const char *name;
	std::string value;
	ad.ResetExpr();
	while (ad.NextExpr(name,expr)) {

		if (!(expr = ad.Lookup(name))) {
			dprintf(D_ALWAYS, "Failed to lookup %s\n", name);

			AbortTransaction();
			text = "Failed to parse job ad attribute";
			return false;
		}

        value = ExprTreeToString(expr);
        ::SetAttribute(cluster, proc, name, value.c_str());
	}

	char buf[22];  
	snprintf(buf, 22, "%d", cluster);
	::SetAttribute(cluster, proc, ATTR_CLUSTER_ID, buf);
	snprintf(buf, 22, "%d", proc);
	::SetAttribute(cluster, proc, ATTR_PROC_ID, buf);
	snprintf(buf, 22, "%ld", time(NULL));
	::SetAttribute(cluster, proc, ATTR_Q_DATE, buf);


	CommitTransaction();


	scheduler.needReschedule();


	MyString tmp;
	tmp.sprintf("%d.%d", cluster, proc);
	id = tmp.Value();

	return true;
}
