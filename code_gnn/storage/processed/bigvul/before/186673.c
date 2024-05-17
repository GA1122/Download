 BackendImpl::BackendImpl(
     const base::FilePath& path,
     scoped_refptr<BackendCleanupTracker> cleanup_tracker,
     const scoped_refptr<base::SingleThreadTaskRunner>& cache_thread,
     net::NetLog* net_log)
     : cleanup_tracker_(std::move(cleanup_tracker)),
       background_queue_(this, FallbackToInternalIfNull(cache_thread)),
       path_(path),
       block_files_(path),
       mask_(0),
       max_size_(0),
       up_ticks_(0),
       cache_type_(net::DISK_CACHE),
       uma_report_(0),
       user_flags_(0),
       init_(false),
       restarted_(false),
       unit_test_(false),
       read_only_(false),
       disabled_(false),
        new_eviction_(false),
        first_timer_(true),
        user_load_(false),
//       consider_evicting_at_op_end_(false),
        net_log_(net_log),
        done_(base::WaitableEvent::ResetPolicy::MANUAL,
              base::WaitableEvent::InitialState::NOT_SIGNALED),
       ptr_factory_(this) {}