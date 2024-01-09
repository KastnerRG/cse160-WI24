#define CATCH_CONFIG_MAIN

#include "gputk.h"
#include "vendor/catch.hpp"



TEST_CASE("Can use basic functions", "[GPUTK]") {
  gpuTKTime_start(GPU, "timer."); //@@ start a timer
  gpuTKTime_stop(GPU, "timer."); //@@ stop the timer

}
