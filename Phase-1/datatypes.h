#ifndef datatypes_h
#define datatypes_h

// header files
#include <stdio.h>

// Global Constants - can be used in other files
typedef enum { False, True } Boolean;

// config data structure
typedef struct ConfigDataType {
    double version;
    char metaDataFileName[ 100 ];
    int cpuSchedCode;
    int quantumCycles;
    int memAvailable;
    int procCycleRate;
    int ioCycleRate;
    char logToFileName[ 100 ];
} ConfigDataType;

typedef struct OpCodeType {
    int pid;
    char command[ 5 ];
    char inOutArg[ 5 ];
    char strArg1[ 15 ];
    int inArg2;
    int inArg3;
    double opEndTime;
    struct OpCodeType * nextNode;
} OpCodeType;

#endif /* datatypes_h */
