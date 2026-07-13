/*
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "server_func.h"

int err_chk (int func, int val, char *func_name)
{
    int status;

    if ((status = func) < val)
    {
        printf ("ERROR: %s error: error value %d \n ", func_name, status);

        return -1;
    }

    return 0;
}

void usage (char *argv0, char *str)
{
    printf ("usage: %s %s \n", argv0, str);
}

int sendall (int fd, char *str, int *len)
{
    int bytesleft = *len; //how many bytes left to send
    int n = 0; 
    int total = 0;  // how many bytes have been sent
    
        while (total < *len)
        {
            n = send (fd, str+total, bytesleft, 0);
            //printf ("n = %d \n", n);

            if (n == -1)
            {
                break;
            }

            total += n;
            bytesleft -= n;
        }
    
        *len = total;    // return number of bytes sent 
    
        return n == -1 ? -1 : 0;    // return -1 on failure, 0 on success
}

int receiveall (int fd, char *str, int *len)
{
    int total = 0;  // how many bytes received
    int bytesleft = *len;
    int n = 0;

    while (total < *len)
    {
        n = recv (fd, str+total, bytesleft, 0 );

        if (n == 0 || n < 0)
        {
            return 0;
        }

        total += n;
        bytesleft -= n;
    }

    *len = total;   // return number recieved
    str[STRBUF] = '\0'; // terminates the end of str 

    return n == -1 ? -1 : 0;
}

enum control getcontrol (char *cmdstr)
{
    int len = strlen (cmdstr);

    for (int i = 0; i < len; i++)
    {
        if (cmdstr[i] == '\n')  // checks for \n which is inserted by fgets 
        {
            cmdstr[i] = '\0';   // replaces \n with \0 to terminate string 
            
             if (strcmp (cmdstr, "start") == 0)
             {
                 return START;
             }
             else if (strcmp (cmdstr, "talk") == 0)
             {
                 return TALK;
             }
             else if (strcmp (cmdstr, "help") || strcmp (cmdstr, "-h") || strcmp (cmdstr, "--help"))
             {
                return HELP; 
             }

        }
    }

    return UNKNOWN_COMMAND;
}

char *charsub (char *str, char find, char sub)
{
    int len = strlen (str);

    for (int i = 0; i < len; i++)
    {
        if (str[i] == find)
        {
            str[i] = sub;

            break;
        }
    }

    return str;
}

bool isaninteger (int num)
{
    if (num == 0 || num == '\0')
    {
        return false;
    }
    else 
    {
        return true;
    }
}

int numbercheck (char *str)
{
    int len = 100;

    for (int i = 0; i < len; i++)
    {
        if (str[i] == '\0' && i < 7)
        {
            len = i - 1;

            for (int i = 0; i < len; i++)
            {
                if (str[i] < 48 || str[i] > 57)
                {
                    return 0;
                }
            }
        }
        else if (i > 6)
        {
            printf ("Enter a port number from range 0 to 65535 \n");

            return 0;
        }
    }

    return 1;
}

int chat (int fd, int new_fd, char *sendstr, char *recstr, int *sendlen, int *reclen)
{
   pthread_t th_threadsendorreceive;    // holds thread ID 
    int loop = 1;  // while loop control 
    int *len = sendlen;     // return status of send and receive
    
    sendall (new_fd, sendstr, len);
    err_chk (pthread_create (&th_threadsendorreceive, NULL, receive_handler, (void*)&new_fd), 0, "thread_create");
    
    while (loop)
    {
        if (strcmp (sendstr, "\x1B\n") == 0)
        {
            close (fd);
            close (new_fd);

            return 0;
        }
        
        //printf (ANSI_FONT_RED"server>>>"ANSI_FONT_BOLD_GREEN"send: \n"ANSI_FONT_RESET);
        printf (ANSI_FONT_RED"server>>> "ANSI_FONT_RESET);
        fgets (sendstr, STRBUF, stdin);

        *len = strlen (sendstr);

        sendall (new_fd, sendstr, len);
        fflush (stdin);
    }

    return 0;
}

int strget (void *arg)
{
    puts (ANSI_FONT_BOLD_GREEN "server>>>"ANSI_FONT_RESET ANSI_FONT_RED"Enter string to send: "ANSI_FONT_RESET);
    
    char str[STRBUF];
    
    fgets (str, STRBUF, stdin);
    
    arg = (char*) str;

    sleep (1);

    return 0;
}

int threadsendorreceive (void *arg)
{
    struct DATA_senrec_t *args;
    args = (struct DATA_senrec_t*) &arg;

    if (args->control == 1)
    {
        err_chk (sendall (args->fd, args->sendstr, &args->len), 0, "sendall");
        
        return 0;
    }
    if (args->control == 2)
    {
        err_chk (receiveall (args->fd, args->recstr, &args->len), 0, "receiveall");
        sleep (1);

        return 0;
    }

    return -1;
}


void *receive_handler (void *sockfd)
{
    int fd = *(int*) sockfd;
    char rx_buffer[STRBUF];
    int len = STRBUF;

    while ((len = recv (fd, rx_buffer, STRBUF - 1, 0)) > 0)
    {
        rx_buffer[STRBUF - 1] = '\0';
        //printf (ANSI_FONT_RED"server>>>"ANSI_FONT_BOLD_GREEN"received: "ANSI_FONT_RESET"%s", rx_buffer);
        printf (ANSI_FONT_RED"\nserver>>>"ANSI_FONT_RESET ANSI_FONT_BLUE"%s\n"ANSI_FONT_RESET ANSI_FONT_RED"server>>> "ANSI_FONT_RESET, rx_buffer);
        fflush(stdout);
        memset (rx_buffer, 0, STRBUF);
    }

    if (len == 0)
    {
        puts ("[Disconnected] \n");
    }
    else 
    {
        perror ("Did not receive");
    }

    close (fd);
    exit (0);
}
