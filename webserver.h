#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

struct ValuePair {
    char* name;
    char* value;
};

struct ResponseContext {
    struct ValuePair *params;
    unsigned int paramSize;
};

int processWebpage(char *page, int pageLen, struct ValuePair *valuePairs, int numOfPairs, char* output);
int start_webserver(int port, struct ResponseContext *context);
void stop_webserver();

#endif /* __WEBSERVER_H__ */
