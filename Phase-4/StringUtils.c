// header files
#include "StringUtils.h"
#include "configops.h"
#include "simtimer.h"

// constant declaration
const char NULL_CHAR = '\0';
const char SPACE = ' ';
const char COLON = ':';
const char COMMA = ',';
const char SEMICOLON = ';';
const char PERIOD = '.';
const char LEFT_PAREN = '(';
const char RIGHT_PAREN = ')';
const int SMALL_STR_LEN = 30;
const int STD_STR_LEN = 60;
const int MAX_STR_LEN = 200;
const int STR_EQ = 0;
const int SUBSTRING_NOT_FOUND = -1001;
const Boolean IGNORE_LEADING_WS = True;
const Boolean ACCEPT_LEADING_WS = False;


int getStringLength (char *str)
{
    int index = 0;
    
    while (str[index] != NULL_CHAR)
    {
        // update index
        index++;
    } // end of the while loop
    
    return index;
}

void copyString (char *destination, char *source)
{
    int index = 0;
    
    while (index < MAX_STR_LEN && source[index] != NULL_CHAR) {
        destination[index] = source[index];
        index++;
        destination[index] = NULL_CHAR;
    } // end of while loop
}


void concatenateString (char *destination, char *source)
{
    int destIndex = getStringLength(destination);
    int sourceIndex = 0;
    
    while (destIndex < MAX_STR_LEN && source[sourceIndex] != NULL_CHAR) {
        destination[destIndex] = source[sourceIndex];
        
        // increment indices
        sourceIndex++;
        destIndex++;
        
        destination[destIndex] = NULL_CHAR;
    } // end of while loop
}


int compareString (char *oneStr, char *otherStr)
{
    int difference, index = 0;
    
    while (index < MAX_STR_LEN && oneStr[index] != NULL_CHAR && otherStr[index] != NULL_CHAR) {
        difference = oneStr[index] - otherStr[index];
        
        // if they're not equal, then return non-zero difference
        if (difference != 0) {
            return difference;
        }
        
        index++;
    }
    
    return getStringLength(oneStr) - getStringLength(otherStr);
}


void getSubString (char *destStr, char *sourceStr, int startIndex, int endIndex)
{
    int sourceStrLen = getStringLength(sourceStr);
    int destIndex = 0;
    int sourceIndex = startIndex;
    char *tempSourceStr;
    
    if (startIndex >= 0 && endIndex >= startIndex && endIndex < sourceStrLen)
    {
        tempSourceStr = (char *) malloc(sourceStrLen + 1);
        copyString(tempSourceStr, sourceStr);
        
        while (destIndex < MAX_STR_LEN && sourceIndex <= endIndex) {
            destStr[destIndex] = tempSourceStr[sourceIndex];
            
            // increment indices
            sourceIndex++;
            destIndex++;
            
            destStr[destIndex] = NULL_CHAR;
        } //end of while loop
        
        free(tempSourceStr);
    } // end of if statement
    else {
        destStr[0] = NULL_CHAR;
    } // end of else statement
}


int findSubString (char *testStr, char *searchSubStr)
{
    // Initialize functions/variables
    int testStrLen = getStringLength(testStr);
    int masterIndex = 0, searchIndex, internalIndex;
    
    while (masterIndex < testStrLen)
    {
        internalIndex = masterIndex;
        searchIndex = 0;
        
        // loop to the end of test string
        while (internalIndex <= testStrLen  && testStr[internalIndex] == searchSubStr[searchIndex])
        {
            // increment test string, substring indices
            internalIndex++;
            searchIndex++;
            
            if (searchSubStr[searchIndex] == NULL_CHAR)
            {
                return masterIndex;
            }
        } // end of the nested while loop
        
        // incerement current beginning location index
        masterIndex++;
    } // end of the main while loop
    
    // assume the test has failed at this point, return SUBSTRING_NOT_FOUND
    return SUBSTRING_NOT_FOUND;
}


void setStrToLowerCase (char *destStr, char *sourceStr)
{
    int strLen = getStringLength(sourceStr);
    char *tempStr = (char *) malloc(strLen + 1);
    int index = 0;
    
    copyString(tempStr, sourceStr);
    
    while (index < MAX_STR_LEN && tempStr[index] != NULL_CHAR) {
        destStr[index] = setCharToLowerCase(tempStr[index]);
        
        // increment index
        index++;
        
        destStr[index] = NULL_CHAR;
    }
    
    free(tempStr);
}

char setCharToLowerCase (char inputChar) {
    if (inputChar >= 'A' && inputChar <= 'Z') {
        inputChar = inputChar - 'A' + 'a';
    }
    
    return inputChar;
}


int getLineTo (FILE *filePtr, int bufferSize, char stopChar, char *buffer, Boolean omitLeadingWhiteSpace, Boolean stopAtNonPrintable)
{
    int charAsInt, charIndex = 0, statusReturn = NO_ERR;
    Boolean bufferSizeAvailable = True, nonPrintableFound = False;
    
    charAsInt = fgetc(filePtr);
    
    while (omitLeadingWhiteSpace == True
           && charAsInt != (int) stopChar
           && charIndex < bufferSize
           && charAsInt <= (int) SPACE) {
        charAsInt = fgetc(filePtr);
    } // end of the while loop
    
    if (stopAtNonPrintable == True && charAsInt < (int) SPACE)
    {
        nonPrintableFound = True;
    } // end of the if statement
    
    while (charAsInt != (int) stopChar && nonPrintableFound == False && bufferSizeAvailable == True)
    {
        if (isEndOfFile(filePtr) == True)
        {
            return INCOMPLETE_FILE_ERR;
        } // end of the if statement
        
        if (charAsInt >= (int) SPACE)
        {
            buffer[charIndex] = (char) charAsInt;
            
            // increment index
            charIndex++;
        } // end of the if statement
        
        buffer[charIndex] = NULL_CHAR;
        
        if (charIndex < bufferSize - 1)
        {
            charAsInt = fgetc(filePtr);
            
            if (stopAtNonPrintable == True && charAsInt < (int) SPACE)
            {
                nonPrintableFound = True;
            } // end of the nested if statement
        } // end of the if statement
        
        else
        {
            bufferSizeAvailable = False;
            statusReturn = INPUT_BUFFER_OVERRUN_ERR;
        }
    }
    
    // return operation status
    return statusReturn;
}

Boolean isEndOfFile (FILE *filePtr)
{
    if (feof(filePtr) != 0)
    {
        return True;
    }
    
    return False;
}

void output(char *message, ConfigDataType *configPtr) {
    if (configPtr->logToCode == LOGTO_FILE_CODE) {
        FILE *fp = fopen(configPtr->logToFileName, "a+");
        fprintf(fp, "%s\n", message);
        fclose(fp);
    }
    
    if (configPtr->logToCode == LOGTO_BOTH_CODE) {
        FILE *fp = fopen(configPtr->logToFileName, "a+");
        fprintf(fp, "%s\n", message);
        fclose(fp);
        printf("%s\n", message);
    }
    
    if (configPtr->logToCode == LOGTO_MONITOR_CODE) {
        printf("%s\n", message);
    }
}

void output_with_time(char *message, ConfigDataType *configPtr) {
    char value[100], buf[200];
    accessTimer(LAP_TIMER, value);
    sprintf(buf, " %s, %s", value, message);
    output(buf, configPtr);
}
