#ifndef BOOTSTRAP_HPP
#define	BOOTSTRAP_HPP
/* A shortcut to `using namespace` statement */
#define UN using namespace
#define NS namespace
/* Define IN-DEPENDENT essensial files */
#include <string>
#include <stdlib.h>
#include <iostream>
/* Global ease of access to std library */
using namespace std;
/* Define useful accessories */
#define echo(o) std::cout<<o<<std::endl<<std::flush
#define disp(o) echo(o)
/* Returns types' value */
#define RETURN_FAILURE  return EXIT_FAILURE
#define RETURN_SUCCESS  return EXIT_SUCCESS
/* TERMINATIONS */
#define TERMINATE_SUCCESS exit(EXIT_SUCCESS)
#define TERMINATE_FAILURE exit(EXIT_FAILURE)
/** PHP DIE() SIMULATOR */
#define die(o) echo(o); TERMINATE_FAILURE
/* Define DEPENDENT essensial files */

#endif	/* BOOTSTRAP_HPP */

