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

        static void trip_set_config(
            std::string const & sec, 
            std::string const & key, 
            std::string const & value)
        {
            TRIP_SetConfig(NULL, sec.c_str(), key.c_str(), value.c_str());
        } 

        static void trip_get_config(
            std::string const & sec, 
            std::string const & key, 
            std::string & value)
        {
            char const * v = TRIP_GetConfig(NULL, sec.c_str(), key.c_str());
            if (v)
                value = v;
        } 

        WorkerModule::WorkerModule(
            util::daemon::Daemon & daemon)
            : util::daemon::ModuleBase<WorkerModule>(daemon, "WorkerModule")
        {
            config().register_ext_config(
                "trip", 
                trip_set_config, 
                trip_get_config);
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
