  CommandBufferCheckLostContext(TransferBufferManager* transfer_buffer_manager,
                                SyncPointManager* sync_point_manager,
                                bool context_lost_allowed)
      : CommandBufferDirect(transfer_buffer_manager, sync_point_manager),
        context_lost_allowed_(context_lost_allowed) {}
