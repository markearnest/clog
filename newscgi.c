/** news.cgi.c
 * CGI to show a specific news item 
 * Inputs:
 *	nid - newsID
 *
 * $Author: mxe20 $
 * $Date: 2005/06/23 02:01:41 $
 * $Header: /home/mxe20/src/clog/RCS/newscgi.c,v 1.3 2005/06/23 02:01:41 mxe20 Exp mxe20 $
 * $Id: newscgi.c,v 1.3 2005/06/23 02:01:41 mxe20 Exp mxe20 $
 * $Locker: mxe20 $
 * $Log: newscgi.c,v $
 * Revision 1.3  2005/06/23 02:01:41  mxe20
 * Modifications for CSS/XHTML 1.1
 *
 * Revision 1.2  2005/05/18 19:21:34  mxe20
 * Check for authentication
 *
 * Revision 1.1  2005/05/11 15:27:02  mxe20
 * Initial revision
 *
 * Revision 1.2  2005/01/10 18:30:13  mxe20
 * Added RCS macros
 *
 * $Name:  $
 * $RCSfile: newscgi.c,v $
 * $Revision: 1.3 $
 * $Source: /home/mxe20/src/clog/RCS/newscgi.c,v $
 * $State: Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include "cloghtml.h"
#include "envar.h"
#include "user.h"
#include "sitevars.h"

int main () {
	int returncode = 0;
	int i;
	char *nid;
	int numQueryString;
	Envar *query;

	cLogInitDB();
	
	/* htmlHeader("News Item"); */
	
	returncode = cLogGetUsername(principalName);

        if(!strcmp(var_site_create_user,"false") && (getenv("REMOTE_USER") != NULL) && returncode != 0) {
                printf("content-type: text/html\r\n\r\n");
                printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">");
                printf("<html><head><title>Logging In</title>");
                printf("<meta http-equiv=\"refresh\" content=\"0;URL=%s\">",var_site_login_url);
                printf("</head><body bgcolor=\"#FFFFFF\">");
                printf("<P>Logging in...<br>\n");
                printf("<P><a href=\"/\">Homepage</a>");
                htmlStaticPrint("footer");
                exit(EXIT_SUCCESS);
        }
                                                                                                                      

	
	if(cLogGetUsername(principalName) >= 20) { 
		htmlHeader("Error");
		printf("<p>Database name lookup failure");
	}
	if(cLogGetGroups(principalName, principalGroups) >= 20) {
		htmlHeader("Error");
		printf("<p>Database group lookup failure");
	}

	query = parseEnVar(getenv("QUERY_STRING"), &numQueryString);
	if(numQueryString == 0) {
		htmlHeader("Error");
		printf("<P>Critical Error: Invalid News Request");
		htmlStaticPrint("footer");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < numQueryString; i++) {
		if(strcmp(query[i].key,"nid") == 0) nid = strdup(query[i].val);
	}

	for (i = 0; i < numQueryString; i++) {
		free(query[i].key);
		free(query[i].val);
	}

	htmlHeader((char*)cLogGetNewsTitle(atoi(nid)));

	htmlLeftSide(principalName, principalGroups);
	htmlStaticPrint("contenttablestart");
	shownews(principalName, principalGroups, nid);
	showcomments(principalName, principalGroups, atoi(nid), 0);
	htmlStaticPrint("contenttableend");
	
	free(nid);
	htmlStaticPrint("footer");
	return 0;
}
