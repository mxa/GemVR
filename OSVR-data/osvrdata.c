/*
 * HOWTO write an External for Pure data
 * (c) 2001-2006 IOhannes m zmölnig zmoelnig[AT]iem.at
 *
 * this is the source-code for the first example in the HOWTO
 * it creates an object that prints "Hello world!" whenever it 
 * gets banged.
 *
 * for legal issues please see the file LICENSE.txt
 */



/**
 * include the interface to Pd 
 */
#include "m_pd.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <osvr/ClientKit/ContextC.h>
#include <osvr/ClientKit/InterfaceC.h>
#include <osvr/ClientKit/InterfaceStateC.h>


/**
 * define a new "class" 
 */
static t_class *osvrdata_class;


/**
 * this is the dataspace of our new object
 * we don't need to store anything,
 * however the first (and only) entry in this struct
 * is mandatory and of type "t_object"
 */
typedef struct _osvrdata {
  t_object  x_obj;
  OSVR_ClientContext x_osvrctx;
  OSVR_ClientInterface x_head;
} t_osvrdata;


/**
 * this method is called whenever a "bang" is sent to the object
 * the name of this function is arbitrary and is registered to Pd in the 
 * osvrdata_setup() routine
 */
void osvrdata_bang(t_osvrdata *x)
{
  /*
   * post() is Pd's version of printf()
   * the string (which can be formatted like with printf()) will be
   * output to wherever Pd thinks it has too (pd's console, the stderr...)
   * it automatically adds a newline at the end of the string
   */
   OSVR_TimeValue timestamp;
   OSVR_PoseState state;
   OSVR_ReturnCode ret;

   osvrClientUpdate(x->x_osvrctx);
   ret = osvrGetPoseState(x->x_head, &timestamp, &state);
   if (OSVR_RETURN_SUCCESS != ret) {
            //No pose state;
            pd_error(x, "couldn't get position");
   } else {
       t_atom arot[4], apos[3];
       SETFLOAT(arot+0, osvrQuatGetX(&state.rotation));
       SETFLOAT(arot+1, osvrQuatGetY(&state.rotation));
       SETFLOAT(arot+2, osvrQuatGetZ(&state.rotation));
       SETFLOAT(arot+3, osvrQuatGetW(&state.rotation));

       SETFLOAT(apos+0, osvrVec3GetX(&state.translation));
       SETFLOAT(apos+1, osvrVec3GetY(&state.translation));
       SETFLOAT(apos+2, osvrVec3GetZ(&state.translation));

       outlet_anything(x->x_obj.ob_outlet, gensym("rotation"), 4, arot);    
       outlet_anything(x->x_obj.ob_outlet, gensym("position"), 3, apos);
    }
}


/**
 * this is the "constructor" of the class
 * this method is called whenever a new object of this class is created
 * the name of this function is arbitrary and is registered to Pd in the 
 * osvrdata_setup() routine
 */
void *osvrdata_new(t_symbol*s)
{
  /*
   * call the "constructor" of the parent-class
   * this will reserve enough memory to hold "t_osvrdata"
   */
  t_osvrdata *x = (t_osvrdata *)pd_new(osvrdata_class);

  x->x_osvrctx = osvrClientInit("com.osvr.osvrexample", 0);
  if(s && s->s_name && *s->s_name) { } else {
      s = gensym("/me/head");
  }
  osvrClientGetInterface(x->x_osvrctx, s->s_name, &x->x_head);

  /*
   * return the pointer to the class - this is mandatory
   * if you return "0", then the object-creation will fail
   */
  outlet_new(&x->x_obj, 0);

  return (void *)x;
}


/**
 * define the function-space of the class
 * within a single-object external the name of this function is special
 */
void osvrdata_setup(void) {
  /* create a new class */
  osvrdata_class = class_new(gensym("osvrdata"),        /* the object's name is "osvrdata" */
			       (t_newmethod)osvrdata_new, /* the object's constructor is "osvrdata_new()" */
			       0,                           /* no special destructor */
			       sizeof(t_osvrdata),        /* the size of the data-space */
			       CLASS_DEFAULT,               /* a normal pd object */
			       A_DEFSYM, 0);                          /* no creation arguments */

  /* attach functions to messages */
  /* here we bind the "osvrdata_bang()" function to the class "osvrdata_class()" -
   * it will be called whenever a bang is received
   */
  class_addbang(osvrdata_class, osvrdata_bang); 
}
