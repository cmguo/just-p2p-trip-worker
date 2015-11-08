// Main.cpp

#include "trip/worker/Common.h"
#include "trip/worker/Version.h"
#include "trip/worker/WorkerModule.h"

#include <framework/logger/Logger.h>
#include <framework/process/Process.h>
#include <framework/process/SignalHandler.h>

#include <boost/bind.hpp>

#ifdef BOOST_POSIX_API
#include <unistd.h>
#include <signal.h>
#endif

namespace trip
{
    namespace worker
    {

        void register_module(util::daemon::Daemon & daemon)
        {
            util::daemon::use_module<trip::worker::WorkerModule>(daemon);
        }

    }
}

#ifndef _LIB

int main(int argc, char * argv[])
{
    util::daemon::Daemon my_daemon("worker.conf");
    char const * default_argv[] = {
        "++framework::logger::Stream.0.file=$LOG/trip_worker.log", 
        "++framework::logger::Stream.0.append=true", 
        "++framework::logger::Stream.0.roll=true", 
        "++framework::logger::Stream.0.level=5", 
        "++framework::logger::Stream.0.size=102400", 
    };
    my_daemon.parse_cmdline(sizeof(default_argv) / sizeof(default_argv[0]), default_argv);
    my_daemon.parse_cmdline(argc, (char const **)argv);

    framework::process::SignalHandler sig_handler(
        framework::process::Signal::sig_int, 
        boost::bind(&util::daemon::Daemon::post_stop, &my_daemon), true);

    framework::logger::load_config(my_daemon.config());

    trip::worker::register_module(my_daemon);

    boost::system::error_code ec;
    my_daemon.start(framework::process::notify_wait, ec);

    return 0;
}

#endif
