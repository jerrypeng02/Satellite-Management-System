#include "webserver.h"
#include <stdio.h>
#include <string.h>

char page[100];
char output[300];
struct ValuePair params[3] = {{"satelliteName", "Explorer"},{"abd", "200"},{"ccc", "333"}};

void testProcessWebpage() {

    strcpy(page, "abcasdasdasd<?satelliteName?>aASDASD<?sadasdasdasd?> <?abd?>?>");
    int pageLen = strlen(page);

    processWebpage(page, pageLen, params, 3, output);

    printf("%s\n", page);
    printf("%s\n", output);
}

int main() {
    struct ResponseContext context;

    context.params = params;
    context.paramSize = 3;

    start_webserver(8888, &context);
    getchar();
    stop_webserver();
}