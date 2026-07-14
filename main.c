/*
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "server_func.h"
#include <netdb.h>
#include <unistd.h>


int main (int argc, char *argv[])
{
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    struct addrinfo hints, *result;
    int sockfd, new_sockfd, len = 0;
    
    // Setting hints memory to 0``
    memset (&hints, 0, sizeof hints);
    
    // Initializing addrinfo structure
    hints.ai_family = AF_UNSPEC;   // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;   // TCP
    hints.ai_flags = AI_PASSIVE;   // fill ip 127.0.0.1
    
    // Server takes a string to send to client and port to bind to as arguments
    if (argc == 3)
    {
        err_chk (getaddrinfo (NULL, argv[2], &hints, &result), 0, "getaddrinfo");
        
        // Error checking for socket creation
		//printf ("ai_family = %d ai_socktype %d ai_protocol %d \n", result->ai_family, result->ai_socktype, result->ai_protocol);
		err_chk ((sockfd = socket (result->ai_family, result->ai_socktype, result->ai_protocol)), 0, "socket");
		// Error checking for binding socket to port
		err_chk (bind (sockfd, result->ai_addr, result->ai_addrlen), 0, "bind");
	   // Error checking for listening on port for incoming connections
		err_chk (listen (sockfd, 10), 0, "listen");

		addr_size = sizeof client_addr;
		
		// Error checking for accepting incoming connections
		err_chk ((new_sockfd = accept (sockfd, (struct sockaddr*)&client_addr, &addr_size)), 0, "accept");
	   
		len = strlen (argv[1]);  // This lenght of the string entered
		
		// Error checking on sendall and sending data done here
		err_chk (sendall (new_sockfd, argv[1], &len), 0, "send");
		  
		printf ("\nBytes sent: %d \n", len);
		printf ("Message sent \n");
		close (sockfd);
		close (new_sockfd);
        
    }
    else if (argc == 2)
    {
        err_chk (getaddrinfo (NULL, MYPORT, &hints, &result), 0, "getaddrinfo");
        
        // Error checking for socket creation
		//printf ("ai_family = %d ai_socktype %d ai_protocol %d \n", result->ai_family, result->ai_socktype, result->ai_protocol);
		err_chk ((sockfd = socket (result->ai_family, result->ai_socktype, result->ai_protocol)), 0, "socket");
		// Error checking for binding socket to port
		err_chk (bind (sockfd, result->ai_addr, result->ai_addrlen), 0, "bind");
	   // Error checking for listening on port for incoming connections
		err_chk (listen (sockfd, 10), 0, "listen");

		addr_size = sizeof client_addr;
		
		// Error checking for accepting incoming connections
		err_chk ((new_sockfd = accept (sockfd, (struct sockaddr*)&client_addr, &addr_size)), 0, "accept");
	   
		len = strlen (argv[1]);  // This lenght of the string entered
		printf ("Strings lenght = %d \n", len);
		// Error checking on sendall and sending data done here
		err_chk (sendall (new_sockfd, argv[1], &len), 0, "send");
		  
		printf ("\nBytes sent: %d \n", len);
		printf ("Message sent \n");
		close (sockfd);
		close (new_sockfd);
    }
    else if (argc == 1)     // If no arguments are passed at terminal application will take commands 
    {
       char cmdbuf[CMDBUF];     // Will take commands 
       char strbuf[STRBUF];     // Will take string to send 
       bool cont = true;    // If true while loop will continue, if false while loop will terminate 
       bool portset = false;    // if port is not set false, if port set true
       int numcheck = 0;

       while (cont)     // Will loop for application to take intructions
       {
           //printf ("\033[31mserver>>>\033[0m ");
           printf (ANSI_FONT_BOLD_GREEN"server>>> "ANSI_FONT_RESET );
           fgets (cmdbuf, CMDBUF, stdin);

            
           // Will check for escape to terminate application
           if (strcmp (cmdbuf, "\x1B\n") == 0)
           {
                cont = false;

                return 0;
           }
            
           // Commands are checked here to run appropriate functions
           switch (getcontrol (cmdbuf))
           {
               // Commands to start the server here with Port number and string to send
                case START:

                    int count = 0;
                    
                    // This loop will take input for Port number and string
                    while (count != 2)
                    {
                        if (!portset)
                        {
                             printf ("\033[31mserver>>>\033[0m \033[1;32mEnter Port Number:\033[0m ");
                             fgets (strbuf, STRBUF, stdin);
                        
                             numcheck = numbercheck (strbuf);   // checks if string contains numercial characters 
                        
                        }
                       
                        if (portset)
                        {
                            printf ("\033[31mserver>>>\033[0m \033[1;32mEnter String to send:\033[0m ");
                            fgets (strbuf, STRBUF, stdin);
                        }
                            
                         //char *endptr;  
                         
                        if (strcmp (strbuf, "\x1B\n") == 0)
                        {
                            //count = 2;
                            //printf ("exit count = %d \n", count);
                            //continue;

                            return 0;
                        }

                        // Will subsittute \n with \0 which was placed in string by fgets()
                        charsub (strbuf, '\n', '\0');
 
                        if (isaninteger (numcheck) && count == 0)
                        {                    
                            err_chk (getaddrinfo (NULL, strbuf, &hints, &result), 0, "getaddrinfo");

                            printf ("\033[1;32mPort set to\033[0m %s \n", strbuf);

                            portset = true;
                        }
                        else if (isaninteger (numcheck) == false && count == 0)
                        {
                             printf ("\033[1;32mEnter a port number\033[0m \n");
                        }
                        
                        if (count == 1)
                        {
                            count++;
                            // Error checking for socket creation
	    	                err_chk ((sockfd = socket (result->ai_family, result->ai_socktype, result->ai_protocol)), 0, "socket");
 
                            // Error checking for binding socket to port
	    	                err_chk (bind (sockfd, result->ai_addr, result->ai_addrlen), 0, "bind");
	                        // Error checking for listening on port for incoming connections  
                            err_chk (listen (sockfd, 10), 0, "listen");

		                    addr_size = sizeof client_addr;
	                     	
	    	                // Error checking for accepting incoming connections
		                    err_chk ((new_sockfd = accept (sockfd, (struct sockaddr*)&client_addr, &addr_size)), 0, "accept");
	   
        // WORK ON CONTINUING TO SEND DATA FROM TO ->
	            	        len = strlen (strbuf);  // This lenght of the string entered
                            int reclen = STRBUF;
                            char recstr[STRBUF];
                            
                            chat (sockfd, new_sockfd, strbuf, recstr, &len, &reclen);
	            	                                   
                            return 0;

                         }
                         
                         // if port number set 
                         if (count == 0 && isaninteger (numcheck))
                         {
                             count++;
                         }
                         else if (count > 0 && isaninteger (numcheck) == false)
                         {
                             printf ("second count inc \n");
                             count++;
                         }             
                }

                case HELP:
                    {
                        printf ("Commands:\nhelp, -h, --help - access help infomation\nstart - starts the server\n");

                        break;
                    }

                default:
                    printf ("\033[1;32mCOMMAND UNKNOWN:\033[0m Type -h or --help for help\n");

            }
               
        }
    }

    else
    {
        usage (argv[0], "[STRING TO SEND] [PORT TO BIND TO]");

        return 1;
    }
	
    return 0;
}
