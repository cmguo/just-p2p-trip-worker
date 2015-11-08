// WorkerModule.h

#ifndef _TRIP_WORKER_WORKER_MODULE_H_
#define _TRIP_WORKER_WORKER_MODULE_H_

#include <util/daemon/Module.h>

namespace trip
{
    namespace worker
    {

        class WorkerModule
            : public util::daemon::ModuleBase<WorkerModule>
        {
        public:
            WorkerModule(
                util::daemon::Daemon & daemon);

            virtual ~WorkerModule();

        public:
            virtual bool startup(
                boost::system::error_code & ec);

            virtual bool shutdown(
                boost::system::error_code & ec);
        };

    } // namespace worker
} // namespace trip

#endif // _TRIP_worker_WORKER_MODULE_H_
