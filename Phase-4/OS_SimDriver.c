// header files
#include "stdio.h"
#include "OS_SimDriver.h"
#include "metadataops.h"
#include "configops.h"
#include "simulator.h"
#include "simtimer.h"

extern const int STR_EQ;

void showProgramFormat();

int main(int argc, char **argv) {
    Boolean programRunFlag = False;
    Boolean configDisplayFlag = False;
    Boolean configUploadFlag = False;
    Boolean mdDisplayFlag = False;
    Boolean runSimFlag = False;
    Boolean infoFlag = False;
    int argIndex = 1;
    int lastFourLetters = 4;
    int fileStrLen, fileStrSubLoc;
    char fileName[STD_STR_LEN];
    char errorMessage[MAX_STR_LEN];
    ConfigDataType *configDataPtr = NULL;
    OpCodeType *metaDataPtr = NULL;
    
    // show title
    // function: printf()
    printf("\nSimulator Program\n");
    printf("=================\n\n");
    
    // check for only one arg
    if (argc < 2) {
        showProgramFormat();
        programRunFlag = False;
        infoFlag = True;
    }
    
    fileName[0] = NULL_CHAR;
    
    while (programRunFlag == False && argIndex < argc) {
        fileStrLen = getStringLength(argv[argIndex]);
        fileStrSubLoc = findSubString(argv[argIndex], ".cnf");
        
        if (compareString(argv[argIndex], "-dc") == STR_EQ) {
            configUploadFlag = True;
            configDisplayFlag = True;
        }
        else if (compareString(argv[argIndex], "-dm") == STR_EQ) {
            configUploadFlag = True;
            mdDisplayFlag = True;
        }
        else if (compareString(argv[argIndex], "-rs") == STR_EQ) {
            configUploadFlag = True;
            runSimFlag = True;
        }
        else if (fileStrSubLoc != SUBSTRING_NOT_FOUND && fileStrSubLoc == fileStrLen - lastFourLetters) {
            copyString(fileName, argv[argIndex]);
            programRunFlag = True;
        }
        
        // increment argIndex
        argIndex++;
    } // end of while loop
    
    if (programRunFlag == False && infoFlag == False) {
        printf("Incorrect argument line format, program aborted\n\n");
        showProgramFormat();
    }
    
    if (programRunFlag == True && configUploadFlag == True) {
        if (getStringLength(fileName) > 0 && getConfigData(fileName, &configDataPtr, errorMessage) == True) {
            if (configDisplayFlag == True) {
                displayConfigData(configDataPtr);
            }
        }
        else {
            printf("\nConfig Upload Error: %s, program aborted\n\n", errorMessage);
            programRunFlag = False;
        }
    }
    
    if (programRunFlag == True && (mdDisplayFlag == True || runSimFlag == True)) {
        if (getMetaData(configDataPtr->metaDataFileName, &metaDataPtr, errorMessage) == True) {
            if (mdDisplayFlag == True) {
                displayMetaData(metaDataPtr);
            }
            
            if (runSimFlag == True) {
                runSim(configDataPtr, metaDataPtr);
            }
        }
        else {
            printf("\nMetadata Upload Error: %s, program aborted\n", errorMessage);
        }
    }
    
    configDataPtr = clearConfigData(configDataPtr);
    metaDataPtr = clearMetaDataList(metaDataPtr);
    
    printf("\nSimulator Program End.\n\n");
    
    return 0;
}

void showProgramFormat() {
    // print out command line argument instructions
    // function: printf
    printf("Program Format:\n");
    printf("\tsim_0x [-dc] [-dm] [-rs] <config file name>\n");
    printf("\t-dc [optional] displays configuration data\n");
    printf("\t-dm [optional] displays meta data\n");
    printf("\t-rs [optional] runs simulator\n");
    printf("\tconfig file name is required\n");
}
