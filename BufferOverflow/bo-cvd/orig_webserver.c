// webserver.c
// FrobozzCo Official Webserver
// Barbazzo Fernap barbazzo@gue.com
// Gustar Woomax gustar@gue.com
// Wilbar Memboob wilbar@gue.com

// By the Frobozz Magic Webserver Company
// Released under the Grue Public License
// Frobruary 14th, 1067 GUE

// THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED
// BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE 
// COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM “AS IS” 
// WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, 
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
// FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY 
// AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE 
// DEFECTIVE, *AND IT WILL*, YOU ASSUME THE COST OF ALL NECESSARY 
// SERVICING, REPAIR OR CORRECTION.

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

#define _XOPEN_SOURCE

typedef struct {
	char *method;
	char *uri;
	char *version;
	char *headers;
} httpreq_t;


/* NOTE: this function is based on a function provided in the GNU "timegm" man
   page. timegm is a GNU extension to time.h that returns the given tm struct as
   a UNIX timestamp in GMT/UTC, rather than local time. The man page suggests a
   function similar to the one below as a portable equivalent.
 */
time_t my_timegm(struct tm *tm) {
	time_t ret;
	char *tz;

	tz = getenv("TZ");
	putenv("TZ=GMT");
	tzset();
	ret = mktime(tm);
	if (tz) {
		char envstr[strlen(tz) + 4];
		envstr[0] = '\0';
		strcat(envstr, "TZ=");
		strcat(envstr, tz);
		putenv(envstr);
	} else {
		putenv("TZ=");
	}

	tzset();

	return ret;
}

char *get_header(const httpreq_t *req, const char* headername) {
	char *hdrptr;
	char *hdrend;
	char *retval = NULL;

	char searchstr[strlen(headername) + 5];
	strcpy(searchstr, "\r\n");
	strcat(searchstr, headername);
	strcat(searchstr, ": ");

	if (hdrptr = strstr(req->headers, searchstr)) { 
		hdrptr += strlen(searchstr); 
		if (hdrend = strstr(hdrptr, "\r\n")) { 
			char hdrval[1024]; // temporary return value
			memcpy((char *)hdrval, hdrptr, (hdrend - hdrptr));
			hdrval[hdrend - hdrptr] = '\0'; // tack null onto end of header value
			int hdrvallen = strlen(hdrval);
			retval = (char *)malloc((hdrvallen + 1) * sizeof(char)); // malloc a space for retval
			strcpy(retval, (char *)hdrval);
		} else {
			retval = (char *)malloc((strlen(hdrptr) + 1) * sizeof(char)); //
			strcpy(retval, hdrptr);
		}
	}

	return retval;
}

/* As long as str begins with a proper HTTP-Version followed by delim, returns a
   pointer to the start of the version number (e.g., 1.0). Returns NULL otherwise.
 */
char *http_version_str(char *str, char *delim) {
	char *vstart = strstr(str, "HTTP/");
	char *vnumstart = str + 5;
	char *vdot = strchr(str, '.');
	char *vend = strstr(str, delim);
	char *digits = "0123456789";
	int majvlen = 0;
	int minvlen = 0;

	if (!vstart || !vdot // something's missing
		|| vstart != str) // str doesn't start with "HTTP/"
		return NULL;

	majvlen = strspn(vnumstart, digits);
	minvlen = strspn(vdot + 1, digits);

	if (majvlen < 1 || (vnumstart + majvlen) != vdot // bad major version
		|| minvlen < 1 || (vdot + minvlen + 1) != vend) // bad minor version
		return NULL;

	return vnumstart;
}

/* Fills req with the request data from datastr. Returns 0 on success.
 */

int parsereq(httpreq_t *req, char *datastr) {
	char *position;
	char *last_position = datastr;
	char *temp_position;
	int matchlen;

	req->method = "";
	req->uri = "";
	req->version = "";
	req->headers = "";

	if (!(position = strchr(last_position, ' '))) {
		return 1;
	}
	matchlen = (int)(position - last_position);
	req->method = (char *)malloc((matchlen + 1) * sizeof(char));
	memcpy(req->method, last_position, matchlen);
	req->method[matchlen] = '\0';
	last_position = position + 1;

	if (!(position = strchr(last_position, ' '))
		&& !(position = strstr(last_position, "\r\n"))) {
		return 1;
	}

	// strip any query string out of the URI
	if ((temp_position = strchr(last_position, '?')) && temp_position < position)
		matchlen = (int)(temp_position - last_position);
	else
		matchlen = (int)(position - last_position);

	req->uri = (char *)malloc((matchlen + 1) * sizeof(char));
	memcpy(req->uri, last_position, matchlen);
	req->uri[matchlen] = '\0';
	if (position[0] == '\r') {
		req->version = "0.9";
		req->headers = "";
		return 0; // simple req -- uri only
	}

	// If we get here, it's a full request, get the HTTP version and headers
	last_position = position + 1;

	if (!(position = strstr(last_position, "\r\n"))
		|| !(last_position = http_version_str(last_position, "\r\n"))) {
		return 1;
	}

	matchlen = (int)(position - last_position);
	req->version = (char *)malloc((matchlen + 1) * sizeof(char));
	memcpy(req->version, last_position, matchlen);
	req->version[matchlen] = '\0';
	last_position = position;

	req->headers = (char *)malloc(strlen(last_position) * sizeof(char));
	strcpy(req->headers, last_position);

	return 0;
} 

char *contype(char *ext) {
	if (strcmp(ext, "html") == 0) return "text/html";
	else if (strcmp(ext, "htm") == 0) return "text/html";
	else if (strcmp(ext, "jpeg") == 0) return "image/jpeg";
	else if (strcmp(ext, "jpg") == 0) return "image/jpeg";
	else if (strcmp(ext, "gif") == 0) return "image/gif";
	else if (strcmp(ext, "txt") == 0) return "text/plain";
	else return "application/octet-stream";

}

char *status(int statcode) {
	if (statcode == 200) 	return "200 OK";
	else if (statcode == 304) return "304 Not Modified";
	else if (statcode == 400) return "400 Bad Request";
	else if (statcode == 403) return "403 Forbidden";
	else if (statcode == 404) return "404 Not Found";
	else if (statcode == 500) return "500 Internal Server Error";
	else if (statcode == 501) return "501 Not Implemented";
	else return "";
}

int send_response(int sockfd, httpreq_t *req, int statcode) {
	int urifd;
	const int BUFSIZE = 1024;
	char sendmessage[BUFSIZE];
	char *path = req->uri;

	if (req->uri == NULL || req->method == NULL || 
		req->headers == NULL || req->version == NULL) {
		return 0;
	}


	if ((path[0] == '/') || ((strstr(path, "http://") == path)
							 && (path = strchr(path + 7,  '/')))) {
		path += 1; // remove leading slash
		if (path[0] == '\0') {  // substituting in index.html for a blank URL!
			path = "index.html";
		} else if (path[strlen(path) - 1] == '/') {
			//concatenating index.html for a /-terminated URL!
			strcat(path, "index.html");    
		}
	} else {
		statcode = 400;
	}

	if (statcode == 200 && (urifd = open(path, O_RDONLY, 0)) < 0) {
		if (errno == ENOENT || errno == ENOTDIR) { // file or directory doesn't exist
			statcode = 404;
		} else if (errno == EACCES) { // access denied
			statcode = 403;
		} else {
			// some other file access problem
			statcode = 500;
		}
	}

	if (strstr(path, "..") != NULL) {
		statcode = 500;
	}


	sendmessage[0] = '\0';
	if (strcmp(req->version, "0.9") != 0) { // full request
		char *ext; // file extension
		time_t curtime;
		char *imstime;
		struct tm tm;
		struct stat stbuf;


		if (statcode == 200) {
			if (ext = strrchr(path, '.')) ext++; // skip the '.'
			else ext = "";
		} else {
			// errors are always html messages
			ext = "html";
		}

		// Conditional GET
		if ((strcmp(req->method, "GET") == 0)
			&& (statcode == 200)
			&& (imstime = get_header(req, "If-Modified-Since"))) {

			// Get statistics about the requested URI from the local filesystem
			if (stat(path, &stbuf) == -1) {
				statcode = 500;
			}

			if (!strptime(imstime, "%a, %d %b %Y %H:%M:%S GMT", &tm)
				&& !strptime(imstime, "%a, %d-%b-%y %H:%M:%S GMT", &tm)
				&& !strptime(imstime, "%a %b %d %H:%M:%S %Y", &tm)) {
				// badly formatted date
				statcode = 400;
			}

			if (stbuf.st_mtime <= my_timegm(&tm)) {
				// Not Modified
				statcode = 304;
			}
		}

		time(&curtime); // time for Date: header
		strcat(sendmessage, "HTTP/1.0 ");
		strcat(sendmessage, status(statcode));    
		strcat(sendmessage, "\r\nDate: ");
		strncat(sendmessage, asctime(gmtime(&curtime)), 24);
		strcat(sendmessage, "\r\nServer: Frobozz Magic Software Company Webserver v.002");
		strcat(sendmessage, "\r\nConnection: close");
		strcat(sendmessage, "\r\nContent-Type: ");    
		strcat(sendmessage, contype(ext));
		strcat(sendmessage, "\r\n\r\n");

	}

	if (statcode != 200) {
		strcat(sendmessage, "<html><head><title>");
		strcat(sendmessage, status(statcode));
		strcat(sendmessage, "</title></head><body><h2>HTTP/1.0</h2><h1>");
		strcat(sendmessage, status(statcode));
		strcat(sendmessage, "</h1><h2>URI: ");
		strcat(sendmessage, path);
		strcat(sendmessage, "</h2></body></html>");
	}

	if (sendmessage[0] != '\0') {
		// send headers as long as there are headers to send
		if (send(sockfd, sendmessage, strlen(sendmessage), 0) < 0) {
			perror("send");
			pthread_exit(NULL);
		}
	}

	if (statcode == 200 && (strcmp(req->method, "HEAD") != 0)) {
		// send the requested file as long as there's no error and the
		// request wasn't just for the headers
		int readbytes;

		while (readbytes = read(urifd, sendmessage, BUFSIZE)) {
			if (readbytes < 0) {
				perror("read");
				pthread_exit(NULL);
			}
			if (send(sockfd, sendmessage, readbytes, 0) < 0) {
				perror("send");
				pthread_exit(NULL);
			}
		}
	}
}

void *data_thread(void *sockfd_ptr) {

	int sockfd = *(int *) sockfd_ptr;
	const int BUFSIZE = 5;
	char recvmessage[BUFSIZE];
	char *headerstr = NULL;
	char *newheaderstr = NULL;
	int recvbytes = 0;
	int curheadlen = 0;
	int totalheadlen = 0;
	httpreq_t req;
	int statcode = 200;
	int done = 0;
	int seen_header = 0;
	char *header_end;
	int content_length = 0;
	char *qstr;

	free(sockfd_ptr); // we have the int value out of this now
	recvmessage[BUFSIZE - 1] = '\0'; // mark end of "string"

	/* Read incoming client message from the socket */  
	while(!done && (recvbytes = recv(sockfd, recvmessage, BUFSIZE - 1, 0))) {
		if (recvbytes < 0) {
			perror("recv");
			pthread_exit(NULL);
		}


		recvmessage[recvbytes] = '\0';

		if (seen_header) {
			// getting the entity body
			content_length -= recvbytes;
			if (content_length <= 0) done = 1;

		} else {

			newheaderstr = (char *) malloc((totalheadlen + recvbytes + 1) * sizeof(char));
			newheaderstr[totalheadlen + recvbytes] = '\0';
			memcpy(newheaderstr, headerstr, totalheadlen);
			memcpy(newheaderstr + totalheadlen, recvmessage, recvbytes);

			if (headerstr != NULL) {
				free(headerstr);
			}

			headerstr = newheaderstr;
			totalheadlen += recvbytes;

			header_end = strstr(headerstr, "\r\n\r\n");

			if (header_end) {
				seen_header = 1;
				header_end[2] = '\0';

				if (parsereq(&req, headerstr) != 0) {
					statcode = 400;
				}

				if (strcmp(req.method, "POST") == 0) {

					// grab the body length
					char *clenstr = get_header(&req, "Content-Length");

					if (clenstr) {

						content_length = atoi(clenstr) - ((headerstr + totalheadlen) - header_end - 4);

						if (content_length <= 0) {
							done = 1;
						}

						free(clenstr);

					} else {

						statcode = 400; // bad request -- no content length
						done = 1;
					}

				} else {

					// This isn't a POST, so there's no entity body
					done = 1;

					if (strcmp(req.method, "GET") != 0
						&& strcmp(req.method, "HEAD") != 0) {

						statcode = 501; // unknown request method
					}

				}
			} // end of "if (header)end)"
		}
	} // end of recv while loop

	// used to deref a NULL pointer here... :(
	if (headerstr != NULL) {
		printf("%s\n", headerstr);
		free(headerstr);
	}

	send_response(sockfd, &req, statcode);
	close(sockfd);

	return NULL;

}

int main(int argc, char *argv[]) {
	int acc, sockfd, clen, port;
	struct hostent *he;
	struct sockaddr_in caddr, saddr;

	if(argc <= 1) {

		fprintf(stderr, "No port specified. Exiting!\n");
		exit(1);

	}

	port = atoi(argv[1]);

	/* Obtain name and address for the local host */
	if((he=gethostbyname("localhost"))==NULL) {

		herror("gethostbyname");
		exit(1);

	}

	/* Open a TCP (Internet Stream) socket */
	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1) {

		perror("socket");
		exit(1);

	}

	/* Create socket address structure for the local host */
	memset((char *) &saddr, '\0', sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(port);
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);

	/* Bind our local address so that the client can send to us */
	if(bind(sockfd,(struct sockaddr *) &saddr,sizeof(saddr)) == -1) {  
		perror("bind");
		exit(1);
	}

	if(listen(sockfd,5) < 0) {
		perror("listen");
		exit(1);
	}

	/* Infinite loop for receiving and processing client requests */
	for(;;) {
		clen=sizeof(caddr);

		/* Wait for a connection for a client process */
		acc=accept(sockfd,(struct sockaddr *) &caddr,(socklen_t*)&clen);
		if(acc < 0) {
			perror("accept");
			exit(1);
		} else {
			pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t));
			int *sockfd_ptr = (int *) malloc(sizeof(int));

			*sockfd_ptr = acc;
			pthread_create(thread, NULL, data_thread, sockfd_ptr);
		}
	}

	return 0;
}
