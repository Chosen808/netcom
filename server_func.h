/*
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef SERVER_FUNC_H
#define SERVER_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
//#include <threads.h>
#include <pthread.h>

#define MYPORT "3490"
#define CMDBUF 50
#define STRBUF 100000

// Font Colors
//      normal fonts 
#define ANSI_FONT_RED "\x1b[31m"
#define ANSI_FONT_GREEN "\x1b[32M"
#define ANSI_FONT_YELLOW "\x1b[33m"
#define ANSI_FONT_BLUE "\x1b[34m"
#define ANSI_FONT_MAGENTA "\x1b[35m"
#define ANSI_FONT_CYAN "\x1b[36m"
#define ANSI_FONT_RESET "\x1b[0m"   // Font reset
//      bold fonts
#define ANSI_FONT_BOLD_RED "\033[1;31m"
#define ANSI_FONT_BOLD_GREEN "\033[1;32m"
#define ANSI_FONT_BOLD_YELLOW "\033[1;33m"
#define ANSI_FONT_BOLD_BLUE "\033[1;34m"
#define ANSI_FONT_BOLD_MAGENTA "\033[1;35m"
#define ANSI_FONT_BOLD_CYAN "\033[1;36m"
#define ANSI_FONT_BOLD_WHITE "033[1;37m"

// Used in converting command strings to integers 
enum control
{
    UNKNOWN_COMMAND,
    START,
    TALK,
    HELP,
} ;

// Data_senrec_t can be used to pass several variables in a multithread function
struct DATA_senrec_t
{
    int fd;     // file descriptor
    char *sendstr;  // string to send
    char *recstr;   // string being received
    int len;    // lenght of strings
    int control;    // control for threadsendorreceive () i for sending, 2 for receiving 
};

/**
 * @brief Error checking for server fucntions 
 *
 * @param func  function for error checking to be done on
 * @param val   value to compare functions return
 * @param func_name     name of function being passed
 *
 * @return int -1 for error, 0 success
 */
int err_chk (int func, int val, char *func_name);


/**
 * @brief Prints out usage information to terminal
 *
 * @param argv0     Program being run 
 * @param str       String to print out to terminal
 */
void usage (char *argv0, char *str);


/**
 * @brief Will send the entire buffer to client
 *
 * @param fd    socket file descriptor
 * @param str   string to send 
 * @param len   lenght of string 
 *
 * @return 0 if success and -1 if failed
 */
int sendall (int fd, char *str, int *len);


/**
 * @brief Will receive entire buffer sent
 *
 * @param fd    socket file desciptor
 * @param str   buffer to hold received string 
 * @param len   lenght of string 
 *
 * @return  0 if success and -1 if failure
 */
int receiveall (int fd, char *str, int *len);


/**
 * @brief Captures control string input
 *
 * @param cmdstr[]  Command string variable
 *
 * @return  return the value of an enum 
 */
enum control getcontrol (char *cmdstr);


/**
 * @brief Will locate a character in a string a substitute it
 *
 * @param str   string to perform substitution on
 * @param find  character to find
 * @param sub   character to substitute with
 *
 * @return returns the string 
 */
char *charsub (char *str, char find, char sub);


/**
 * @brief Will test if a string or number was returned from strtol
 *
 * @param num variable to test
 *
 * @return will return an integer
 */
bool isaninteger (int num);


/**
 * @brief Will check if a string contains number characters only
 *
 * @param str   string to check
 *
 * @return  0 if string contains letter, 1 if string only has numbers 
 */
int numbercheck (char *str);



/**
 * @brief Initates a continuos send and receive session
 *
 * @param fd    socket() file descriptor 
 * @param new_fd    accept() file descriptor 
 * @param sendstr   string to send data from 
 * @param recstr    string to receive data 
 * @param sendlen   lenght of sendstr 
 * @param reclen    size of the receive string buffer 
 *
 * @return  -1 if failure, 0 if success
 */
int chat (int fd, int new_fd, char *sendstr, char *recstr, int *sendlen, int *reclen);


/**
 * @brief This function will take stdin (string input from)
 *
 * @param arg   string to pass 
 *
 * @return returns 1
 */
int strget (void *arg);


/**
 * @brief This function takes argments for sendall() or receiveall() to be
 * used to multithread. This function must be passed to thrd_create()
 *
 * @param arg   Takes a struct DATA_senrec_t 
 *
 * @return -1 if failure, 0 if success 
 */
int threadsendorreceive (void *arg);


/**
 * @brief Used to receive incoming data in a thread 
 *
 * @param sockfd    socket file descriptor 
 *
 * @return void no return
 */
void *receive_handler (void *sockfd);


/**
 * @brief This function with print helper information about the application
 *
 * @param argv  string that contains help request command
 *
 * @return  returns true or false 
 */
bool get_help (char *argv[]);


/**
 * @brief Prints help information about using the application
 *
 * @param argv[]    string that contains help request command
 *
 * @return 0
 */
int help (char *argv[]);
#endif // SERVER_FUNC_H
