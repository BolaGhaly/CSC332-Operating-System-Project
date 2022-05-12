// header files
#include "StringUtils.h"

// constant declaration
const char NULL_CHAR = '\0';
const char SPACE = ' ';
const char COLON = ':';
const char SEMICOLON = ';';
const char COMMA = ',';
const char PERIOD = '.';
const int STD_STR_LEN = 64;
const int MAX_STR_LEN = 128;
const int STR_EQ = 0;
const int SUBSTRING_NOT_FOUND = -1001;
const Boolean IGNORE_LEADING_WS = True;
const Boolean ACCEPT_LEADING_WS = False;

int compareString (const char *oneStr, const char *otherStr)
{
    int diff, index = 0;
    
    while (oneStr[index] != NULL_CHAR && otherStr[index] != NULL_CHAR && index < MAX_STR_LEN)
    {
        diff = oneStr[index] - otherStr[index];
        
        if (diff != 0)
        {
            return diff;
        }
        
        //increment index
        index++;
    } // end of the while loop
    
    return getStringLength(oneStr) - getStringLength(otherStr);
}

void concatenateString (char *destStr, const char *sourceStr)
{
    int destIndex = getStringLength(destStr);
    
    int sourceStrLen = getStringLength(sourceStr);
    
    char *tempStr;
    
    int sourceIndex = 0;
    
    tempStr = (char *) malloc(sizeof(sourceStrLen + 1));
    copyString(tempStr, sourceStr);
    
    while (tempStr[sourceIndex] != NULL_CHAR && destIndex < MAX_STR_LEN) {
        destStr[destIndex] = tempStr[sourceIndex];
        
        // update indices
        destIndex++;
        sourceIndex++;
        
        destStr[destIndex] = NULL_CHAR;
    } // end of the while loop
    
    free(tempStr);
}

void copyString (char * destStr, const char *sourceStr)
{
    int index = 0;
    
    if (destStr != sourceStr)
    {
        while (sourceStr[index] != NULL_CHAR && index < MAX_STR_LEN)
        {
            destStr[index] = sourceStr[index];
            
            index++;
            
            destStr[index] = NULL_CHAR;
        } // end of the while loop
    }
}


int findSubString (const char *testStr, const char *searchSubStr)
{
    // Initialize functions/variables
    int testStrLen = getStringLength(testStr);
    int masterIndex = 0;
    int searchIndex, internalIndex;
    
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


bool getStringConstrained (FILE *inStream, bool clearLeadingNonPrintable, bool clearLeadingSpace, bool stopAtNonPrintable, char delimiter, char *capturedString)
{
    // Initialize variables
    int intChar = EOF, index = 0;
    
    // Initialize output string
    capturedString[index] = NULL_CHAR;
    
    // capture first value in stream
    // function: fgetc
    intChar = fgetc(inStream);
    
    while (((intChar != EOF) && ((clearLeadingNonPrintable && intChar == (int)SPACE)))
           || (clearLeadingSpace && intChar == (int)SPACE))
    {
        intChar = fgetc(inStream);
    }
    
    if (intChar == EOF)
    {
        // return failed operation
        return false;
    }
    
    //loop to capture input
    while ((intChar != EOF && index < MAX_STR_LEN - 1) && ((stopAtNonPrintable && intChar >= (int)SPACE) || (!stopAtNonPrintable)) && (intChar != (char)delimiter))
    {
        capturedString[index] = (char)intChar;
        
        index++;
        
        capturedString[index] = NULL_CHAR;
        
        intChar = fgetc(inStream);
    } // end of the while loop
    
    return true;
}

int getStringLength (const char *teststr)
{
    int index = 0;
    
    while (index < STD_STR_LEN && teststr[index] != NULL_CHAR)
    {
        // update index
        index++;
    } // end of the while loop
    
    return index;
}

bool getStringToDelimiter (FILE *inStream, char delimiter, char *capturedString)
{
    return getStringConstrained(inStream, true, true, true, delimiter, capturedString);
}

bool getStringToLineEnd (FILE *inStream, char *capturedString)
{
    return getStringConstrained(inStream, true, true, true, NON_PRINTABLE_CHAR, capturedString);
}

void getSubString (char *destStr, const char *sourceStr, int startIndex, int endIndex)
{
    int sourceStrLen = getStringLength(sourceStr);
    int destIndex = 0;
    int sourceIndex = startIndex;
    char *tempStr;
    
    if (startIndex >= 0 && startIndex <= endIndex && endIndex < sourceStrLen)
    {
        tempStr = (char *) malloc(sourceStrLen + 1);
        copyString(tempStr, sourceStr);
        
        while (sourceIndex <= endIndex) {
            destStr[destIndex] = tempStr[sourceIndex];
            
            // update indices
            destIndex++;
            sourceIndex++;
            
            destStr[destIndex] = NULL_CHAR;
        } // end of the while loop
        
        free(tempStr);
    }
}

void setStrToLowerCase (char *destStr, const char *sourceStr)
{
    int sourceStrLen = getStringLength(sourceStr);
    
    char *tempStr;
    
    int index = 0;
    
    tempStr = (char *) malloc(sizeof(sourceStrLen + 1));
    copyString(tempStr, sourceStr);
    
    while (tempStr[index] != NULL_CHAR && index < MAX_STR_LEN) {
        destStr[index] = toLowerCase(tempStr[index]);
        
        index++;
        
        destStr[index] = NULL_CHAR;
    } // end of the while loop
    
    free(tempStr);
}

char toLowerCase (char testChar)
{
    // check for uppercase letter
    if (testChar >= 'A' && testChar <= 'Z')
    {
        // return lower case letter
        testChar = testChar - 'A' + 'a';
    }
    
    return testChar;
}

int getLineTo (FILE *filePtr, int bufferSize, char stopChar, char *buffer, Boolean omitLeadingWhiteSpace, Boolean stopAtNonPrintable)
{
    int charAsInt, charIndex = 0;
    int statusReturn = NO_ERR;
    Boolean bufferSizeAvailable = True;
    Boolean nonPrintableFound = False;
    
    charAsInt = fgetc(filePtr);
    
    while (omitLeadingWhiteSpace == True && charAsInt != (int) stopChar && charIndex < bufferSize && charAsInt <= (int) SPACE) {
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
            
            if (stopAtNonPrintable == True && charAsInt < (int)SPACE)
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

