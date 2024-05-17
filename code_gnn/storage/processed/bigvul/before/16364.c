pseudo_constrain( const char *expr )
{
	MyString reqs;
	MyString newreqs;

	dprintf(D_SYSCALLS,"pseudo_constrain(%s)\n",expr);
	dprintf(D_SYSCALLS,"\tchanging AgentRequirements to %s\n",expr);
	
	if(pseudo_set_job_attr("AgentRequirements",expr)!=0) return -1;
	if(pseudo_get_job_attr("Requirements",reqs)!=0) return -1;

	if(strstr(reqs.Value(),"AgentRequirements")) {
		dprintf(D_SYSCALLS,"\tRequirements already refers to AgentRequirements\n");
		return 0;
	} else {
		newreqs.sprintf("(%s) && AgentRequirements",reqs.Value());
		dprintf(D_SYSCALLS,"\tchanging Requirements to %s\n",newreqs.Value());
		return pseudo_set_job_attr("Requirements",newreqs.Value());
	}
}
