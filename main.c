#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SQL_BUFFER_SIZE 100
const char* replacePhrase = "&-SUB-&";

enum DatabaseType {
    DBMS1 = 1,
    DBMS2,
    DBMS3
};

char* get_substr_name(const enum DatabaseType dbms) {
    switch (dbms) {
        case DBMS1:
            return "SUBSTR";
        case DBMS2:
        default:
            return "SUBSTRING";
        case DBMS3:
            return "SBSTR";
    }
}
/**
 * @brief Replaces substring SQL function name with the requested value
 * Callee *MUST* free memory of returned string.
 * Input param string is not modified nor freed
 *
 * @param inputString Original SQL char* string with replacement phrase tag
 * @param database The databaseType enum value to use
 * @return new string with replacement value or NULL
 */
/* ASSUMPTIONS
 * input contains a valid, null-terminated string, sanitized, and has parse phrase inserted in valid location.
 * this function only does string substitution.
 */
char* replace_substr(const char* inputString, const enum DatabaseType database) {
    if (!inputString) {
        return NULL;    // Nothing to do
    }
    // Get replacement function name
    char* substringName = get_substr_name(database);
    size_t substringNameLength = strlen(substringName);
    size_t replacePhraseLength = strlen(replacePhrase);

    // Calculate new string length and allocate memory for it
    size_t newStringLength = strlen(inputString) + substringNameLength - replacePhraseLength + 1;
    char* retStr = (char *)malloc(newStringLength * sizeof(char));
    if (!retStr) {
        return NULL;
    }

    // Copy original string to start of replacement string phrase
    char* start = strstr(inputString, replacePhrase);
    strncpy(retStr, inputString,  start - inputString);

    // Copy new SQL function name string value in
    char *currPtr = retStr + (start - inputString);
    strncpy(currPtr, substringName, substringNameLength);

    // Copy rest of original string
    currPtr += substringNameLength;
    strncpy(currPtr, start + replacePhraseLength, strlen(start) -  replacePhraseLength + 1);

    return  retStr;
}

int main(int argc, char** argv) {
    enum DatabaseType dbType = DBMS1;

    if (argc > 1) {
        dbType = atoi(argv[1]);
    }

    char buffer[SQL_BUFFER_SIZE];
    snprintf(buffer, SQL_BUFFER_SIZE, "SELECT %s(Model, 1, 3) FROM SASHELP.CARS", replacePhrase);

    char* newBuffer = replace_substr(buffer, dbType);
    if (newBuffer) {
        printf("%s\n", newBuffer);
        // Do something magical here

        // Cleanup
        free(newBuffer);
    }
    return 0;
}
