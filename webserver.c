/*
     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
 * a part of the file comes from Christian Grothoff
 */

#include "webserver.h"

#include "platform.h"
#include <microhttpd.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */

// added
#include <string.h>

#ifndef S_ISREG
#define S_ISREG(x) (S_IFREG == (x & S_IFREG))
#endif /* S_ISREG */

int processWebpage(char *page, int pageLen, struct ValuePair valuePairs[], int numOfPairs, char *output)
{
    volatile int i;
    int len;
    static char name[1024];
    char *needle = page;
    char *wNeedle = output;
    char *start, *end;

    while (1)
    {
        start = strstr(needle, "<?");
        if (start == NULL)
            break;
        end = strstr(start, "?>");
        if (end == NULL)
            break;

        printf("%ld %ld\n", start - page, end - page);

        if (end - start < 100)
        {
            strncpy(name, start + 2, end - start - 2);
            name[end - start - 2] = '\0';
            printf("%ld\n", end - start);
            printf("%s\n", name);

            char match = -1;
            for (i = 0; i < numOfPairs; i++)
            {
                if (0 == strcmp(name, valuePairs[i].name))
                {
                    match = i;
                    break;
                }
            }
            if (match == -1)
            {
                strncpy(wNeedle, needle, end + 2 - needle);
                wNeedle += end + 2 - needle;
            }
            else
            {
                strncpy(wNeedle, needle, start - needle);
                wNeedle += start - needle;
                len = strlen(valuePairs[match].value);
                strncpy(wNeedle, valuePairs[match].value, len);
                wNeedle += len;
            }
        }
        else
        {
            strncpy(wNeedle, needle, end + 2 - needle);
            wNeedle += end + 2 - needle;
        }
        needle = end + 2;
    }

    strncpy(wNeedle, needle, pageLen - (needle - page));
    wNeedle += pageLen - (needle - page);
    *wNeedle = '\0';

    return wNeedle - output;
}

static int
ahc_echo(void *cls,
         struct MHD_Connection *connection,
         const char *url,
         const char *method,
         const char *version,
         const char *upload_data,
         size_t *upload_data_size, void **ptr)
{
    struct ResponseContext *context = (struct ResponseContext *)cls;

    static int aptr;
    struct MHD_Response *response;
    int ret;

    char *output;
    static char filename[1024];

    int fd;
    struct stat buf;

    if ((0 != strcmp(method, MHD_HTTP_METHOD_GET)) &&
        (0 != strcmp(method, MHD_HTTP_METHOD_HEAD)))
        return MHD_NO; /* unexpected method */
    if (&aptr != *ptr)
    {
        /* do never respond on first call */
        *ptr = &aptr;
        return MHD_YES;
    }
    *ptr = NULL; /* reset when done */
    /* WARNING: direct usage of url as filename is for example only!
    * NEVER pass received data directly as parameter to file manipulation
    * functions. Always check validity of data before using.
    */

    //printf("received");

    int status = 0;
    /**
     * status -1 = error
     * status 0 = output buffer
     * status 1 = fd file
     **/

    if (NULL != strstr(url, "../"))                         /* Very simplified check! */
        status = -1;                                        /* Do not allow usage of parent directories. */
    else if (strcmp(url, "") == 0 || strcmp(url, "/") == 0) /* homepage */
    {
        sprintf(filename, "web/index.html");
    } else {
        sprintf(filename, "web%s", url);
    }

    if (status != -1) {
        if (NULL != strstr(filename, ".html")) {

/*
            FILE *logF = fopen("web.log","w");
            fprintf(logF, "%s", filename);
            fflush(logF);*/

            FILE *f = fopen(filename, "rb");
            if (f != NULL) {
                fseek(f, 0, SEEK_END);
                long fsize = ftell(f);
                fseek(f, 0, SEEK_SET); //same as rewind(f);
                char *fstring = malloc(fsize + 1);
                output = (char *)malloc(fsize + context->paramSize * 100);
                fread(fstring, fsize, 1, f);
                fclose(f);
                fstring[fsize] = 0;

                processWebpage(fstring, fsize, context->params, context->paramSize, output);

                //fprintf(logF, "%s", output);
                //fflush(logF);

                free(fstring);
                status = 0;
            } else {
                status = -1;
                fd = -1;
            }
            //fclose(logF);
        }
        else
        {
            fd = open(filename, O_RDONLY);

            if (fd != -1)
            {
                if ((0 != fstat(fd, &buf)) ||
                    (!S_ISREG(buf.st_mode)))
                {
                    /* not a regular file, refuse to serve */
                    if (0 != close(fd))
                        abort();
                    status = -1;
                }
                status = 1;
            }
            else
            {
                status = -1;
            }
        }
    }

    if (status == -1)
    {
        fd = open("web/error.html", O_RDONLY);
        status = 1;
    }

    if (status == 0) // if buffer
    {
        response = MHD_create_response_from_buffer(strlen(output),
                                                   (void *)output,
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        free(output);
    }
    else if (status == 1 && fd != -1) // if file
    {
        response = MHD_create_response_from_fd64(buf.st_size, fd);
        if (NULL == response)
        {
            if (0 != close(fd))
                abort();
            return MHD_NO;
        }
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
    }
    else
    {
        // Exception
    }
    return ret;
}

struct MHD_Daemon *d;

int start_webserver(int port, struct ResponseContext *context)
{
    d = MHD_start_daemon( // MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,
        MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,
        // MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | MHD_USE_POLL,
        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | MHD_USE_POLL,
        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,
        port,
        NULL, NULL, &ahc_echo, (void *)context,
        MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int)120,
        MHD_OPTION_STRICT_FOR_CLIENT, (int)1,
        MHD_OPTION_END);
    if (d == NULL)
        return 1;
    return 0;
}

void stop_webserver()
{
    MHD_stop_daemon(d);
}
