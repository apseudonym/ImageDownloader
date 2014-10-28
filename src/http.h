#define GET 10
#define HEAD 11
#define CRLF "\r\n"

char* genrequest(const char* input, const char* domain, int request);

int parsehead(char* resp);

int processchunked(char* resp);

char* findheadend(char* resp);
