/** comments.cgi.c
 * CGI to display a specific comment (with or without children comments)
 *	input parms:
 *		pid = ParentID  \ Show all childern of parentID
 *		nid = NewsID	/
 *	OR
 *		cid = commentID - Show comment by itself
 * 
 * $Author: mxe20 $
 * $Date: 2005/06/22 16:09:13 $
 * $Header: /home/mxe20/src/clog/RCS/commentscgi.c,v 1.2 2005/06/22 16:09:13 mxe20 Exp mxe20 $
 * $Id: commentscgi.c,v 1.2 2005/06/22 16:09:13 mxe20 Exp mxe20 $
 * $Locker: mxe20 $
 * $Log: commentscgi.c,v $
 * Revision 1.2  2005/06/22 16:09:13  mxe20
 * Modifications for XHTML/CSS
 *
 * Revision 1.1  2005/05/11 15:27:02  mxe20
 * Initial revision
 *
 * Revision 1.2  2005/01/10 18:30:13  mxe20
 * Added RCS macros
 *
 * $Name:  $
 * $RCSfile: commentscgi.c,v $
 * $Revision: 1.2 $
 * $Source: /home/mxe20/src/clog/RCS/commentscgi.c,v $
 * $State: Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cloghtml.h"
#include "clogdb.h"
#include "user.h"
#include "urldec.h"
#include "showcomments.h"

int main () {
  char *nid = NULL;	/* newsID */
	char *pid = NULL;	/* parentID */
	struct field_rec *p;
	
	cLogInitDB();
	
	htmlHeader("Comment");
		
	if(cLogGetUsername(principalName) >= 20) 
		printf("<p>Database name lookup failure");
	if(cLogGetGroups(principalName, principalGroups) >= 20)
		printf("<p>Database group lookup failure");

	/* http get */
	urlDecode(getenv("QUERY_STRING"));	
	p=urldec;
	while (p) {
		if(strcmp(p->field_name,"pid") == 0) 
			pid = strdup(p->field_data);
		if(strcmp(p->field_name,"nid") == 0)
			nid = strdup(p->field_data);
		p=p->next;
	}


	htmlLeftSide(principalName, principalGroups);
	htmlStaticPrint("contenttablestart");

	if(pid != NULL && nid != NULL) {
		showcomments(principalName, principalGroups, atoi(nid), atoi(pid));
	/* in all other cases, panic */
	} else {
		printf("<P>Critical Error: Parm mismatch");
	}

	htmlStaticPrint("contenttableend");
	htmlStaticPrint("footer");

	if(nid != NULL) free(nid);
	if(pid != NULL) free(pid);
		
	return 0;

}
