#include <stdio.h>
#include <stdlib.h>
#include <tk.h>
#include "photo.hh"

extern "C" {
  int Inti_Init(Tcl_Interp *interp);
	   };

extern "C" {
  int Inti_SafeInit(Tcl_Interp *interp);
	   };


extern int Inti_Init(Tcl_Interp *interp) {
	Tcl_CreateCommand(interp, "Intinew", Inti_new, (ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);

	init_retcc();
	return TCL_OK;
}

extern int Inti_SafeInit(Tcl_Interp *interp) {
       char *error_msg;

       sprintf(error_msg,"Safe interpreter not supported");
       Tcl_SetResult(interp,error_msg,TCL_VOLATILE);
       return TCL_ERROR;
}
