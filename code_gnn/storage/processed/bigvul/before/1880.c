int reds_handle_migrate_data(MainChannelClient *mcc, SpiceMigrateDataMain *mig_data, uint32_t size)
{
    VDIPortState *agent_state = &reds->agent_state;

     
    if (reds->mm_timer_enabled) {
        reds_send_mm_time();
    }
    if (mig_data->agent_base.connected) {
        if (agent_state->base) {  
            if (!vdagent) {
                spice_assert(agent_state->plug_generation > 0);
                main_channel_push_agent_disconnected(reds->main_channel);
                spice_debug("agent is no longer connected");
            } else {
                if (agent_state->plug_generation > 1) {
                     
                    spice_debug("agent has been detached and reattached before receiving migration data");
                    main_channel_push_agent_disconnected(reds->main_channel);
                    main_channel_push_agent_connected(reds->main_channel);
                } else {
                    return reds_agent_state_restore(mig_data);
                }
            }
        } else {
             
            spice_assert(agent_state->plug_generation == 0);
            agent_state->mig_data = spice_memdup(mig_data, size);
        }
    } else {
        if (vdagent) {
             
            spice_char_device_client_remove(agent_state->base,
                                            main_channel_client_get_base(mcc)->client);
            main_channel_push_agent_connected(reds->main_channel);
        }
    }

    return TRUE;
}