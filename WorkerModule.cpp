// WorkerModule.cpp

#include "trip/worker/Common.h"
#include "trip/worker/WorkerModule.h"
#include "trip/worker/Name.h"

#include <trip/client/Name.h>
#include <trip/client/export/Client.h>

#include <framework/process/Process.h>
#include <framework/process/Environments.h>
#include <framework/network/NetName.h>
#include <framework/filesystem/Path.h>
#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/system/LogicError.h>

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
using namespace boost::system;


FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("trip.worker.WorkerModule", framework::logger::Debug);

namespace trip
{
    namespace worker
    {

        WorkerModule::WorkerModule(
            util::daemon::Daemon & daemon)
            : util::daemon::ModuleBase<WorkerModule>(daemon, "WorkerModule")
        {
        }

        WorkerModule::~WorkerModule()
        {
        }

        bool WorkerModule::startup(
            error_code & ec)
        {
            int ret = TRIP_StartEngine();
            if (ret == 0) {
                char const * addr = TRIP_GetConfig(NULL, "trip.client.HttpManager", "addr");
                framework::network::NetName name(addr);
                framework::process::set_environment(ENVIRON_HTTP_PORT, name.svc());
            }
            return ret == 0;
        }

        bool WorkerModule::shutdown(
            error_code & ec)
        {
            TRIP_StopEngine();
            return true;
        }

    } // namespace worker
} // namespace trip
