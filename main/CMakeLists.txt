idf_component_register(SRCS
        "rf.cpp"
        interfaces/Task.h
        interfaces/Task.cpp

        "livolo.h"
        "livolo.cpp"
        "reset_task.h"
        "reset_task.cpp"
        common.h
        WifiTask.h
        WifiTask.cpp
        INCLUDE_DIRS "")