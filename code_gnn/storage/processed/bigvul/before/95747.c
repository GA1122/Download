void CL_VoipParseTargets(void)
{
	const char *target = cl_voipSendTarget->string;
	char *end;
	int val;

	Com_Memset(clc.voipTargets, 0, sizeof(clc.voipTargets));
	clc.voipFlags &= ~VOIP_SPATIAL;

	while(target)
	{
		while(*target == ',' || *target == ' ')
			target++;

		if(!*target)
			break;
		
		if(isdigit(*target))
		{
			val = strtol(target, &end, 10);
			target = end;
		}
		else
		{
			if(!Q_stricmpn(target, "all", 3))
			{
				Com_Memset(clc.voipTargets, ~0, sizeof(clc.voipTargets));
				return;
			}
			if(!Q_stricmpn(target, "spatial", 7))
			{
				clc.voipFlags |= VOIP_SPATIAL;
				target += 7;
				continue;
			}
			else
			{
				if(!Q_stricmpn(target, "attacker", 8))
				{
					val = VM_Call(cgvm, CG_LAST_ATTACKER);
					target += 8;
				}
				else if(!Q_stricmpn(target, "crosshair", 9))
				{
					val = VM_Call(cgvm, CG_CROSSHAIR_PLAYER);
					target += 9;
				}
				else
				{
					while(*target && *target != ',' && *target != ' ')
						target++;

					continue;
				}

				if(val < 0)
					continue;
			}
		}

		if(val < 0 || val >= MAX_CLIENTS)
		{
			Com_Printf(S_COLOR_YELLOW "WARNING: VoIP "
				   "target %d is not a valid client "
				   "number\n", val);

			continue;
		}

		clc.voipTargets[val / 8] |= 1 << (val % 8);
	}
}
