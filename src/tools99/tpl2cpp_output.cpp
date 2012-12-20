// $Id$
/*
 *
 * Author: Hans J. Skaug
 * Copyright (c) 2009-2012 ADMB foundation
 */

/**
  \file tpl2cpp_output.cpp
  \ingroup prepros
  \brief Typical output from the preprosessor "tpl2cpp". 
  This file is generated by the command

    tpl2cpp tpl2cpp_output

  in a case case there the model is contained in "tpl2cpp_output.tpl". 
  Hence the contents of tpl2cpp_output.cpp is model specific, 
  and only generic aspects are documented.
 */


/**
 * Allocates variables defined in the DATA_SECTION of the tpl-file.
 * \param argc number of command line arguments (in a standard C sense)
 * \param argv string of command line arguments (in a standard C sense)
 *
 * Called from the constructor of "model_parameters".
 */
model_data::model_data(int argc,char * argv[]) : ad_comm(argc,argv)
{
  n.allocate("n");
  Y.allocate(1,10,1,10,"Y");
  y2 = 10.0;
}

/**
 * Performes initilizations declared in the INITIALIZATION_SECTION of the tpl-file.
 */
void model_parameters::initializationfunction(void)
{
  gamma.set_initial_value(1.0);
}

/**
 * Allocate and initialize model parameters defined in the PARAMETER_SECTION of the tpl-file.
 * \param argc number of command line arguments (in a standard C sense)
 * \param argv string of command line arguments (in a standard C sense)
 */
model_parameters::model_parameters(int sz,int argc,char * argv[]) : 
 model_data(argc,argv) , function_minimizer(sz)
{
  initializationfunction();
  alpha.allocate(1,3,1,"alpha");
  beta.allocate(1,3,-10000.0,10000.0,2,"beta");
  gamma.allocate(-6.0,6.0,"gamma");
  x1.allocate("x1");
  #ifndef NO_AD_INITIALIZE
  x1.initialize();
  #endif
  x2.allocate("x2");
  x3.allocate(1,4,"x3");
  l.allocate("l");
  prior_function_value.allocate("prior_function_value");
  likelihood_function_value.allocate("likelihood_function_value");
}

/**
 * Performes calculations defined in the PRELIMINARY_CALCS_SECTION of the tpl-file.
 */
void model_parameters::preliminary_calculations(void)
{

  admaster_slave_variable_interface(*this);
  Y = 10.0;
  penalty = 1.0;
}

/**
 * Performes calculations defined in the BETWEEN_PHASES_SECTION of the tpl-file.
 */
void model_parameters::between_phases_calculations(void)
{
  switch (current_phase())
  {
    case 1:
      penalty = 0.001;
    break;
    case 2:
      penalty = 0.001;
    break;
  }
}

/**
 * Performes calculations defined in the PROCEDURE_SECTION of the tpl-file.
 * The initialization "l=0.0" of the objective function is autogenerated by tpl2cpp.
 */
void model_parameters::userfunction(void)
{
  l =0.0;
  my_function();
  l = -norm2(beta);
}

/**
 * Example of user defined function resulting from a FUNCTION section in the tpl-file.
 */
void model_parameters::my_function(void)
{
  x1 = x1 + 1.0;
}

/**
 * Performes calculations defined in the PROCEDURE_SECTION of the tpl-file.
 * \param
 */
void model_parameters::set_runtime(void)
{
  dvector temp("{.1, .1, .001}");
  convergence_criteria.allocate(temp.indexmin(),temp.indexmax());
  convergence_criteria=temp;
  dvector temp1("{20, 20, 1000}");
  maximum_function_evaluations.allocate(temp1.indexmin(),temp1.indexmax());
  maximum_function_evaluations=temp1;
}

/**
 * Performes calculations defined in the FINAL_SECTION of the tpl-file.
 * \param
 */
void model_parameters::final_calcs()
{
}

/**
 * Performes calculations defined in the REPORT_SECTION of the tpl-file.
 * \param
 */
void model_parameters::report()
{
 adstring ad_tmp=initial_params::get_reportfile_name();
  ofstream report((char*)(adprogram_name + ad_tmp));
  if (!report)
  {
    cerr << "error trying to open report file"  << adprogram_name << ".rep";
    return;
  }
}


  long int arrmblsize=0;

/**
 * Entry point for the program (as in standard C/C++).
 * \param argc number of command line arguments (in a standard C sense).
 * \param argv string of command line arguments (in a standard C sense).
 *
 * This is the function in which the execution of the model starts
 * (and ends if excecution goes normally). Main activities:
 * 
 * i) Create an object "mp" containing all model parameters. This
 *    implicitely involves the two subtasks:
 *    - Setting up the model data object
 *    - Setting up the function minimizer object.
 *
 * ii) Performes preliminary calculations (on the data).
 *
 * ii) Performes function minimization.
 */
int main(int argc,char * argv[])
{
  ad_set_new_handler();
  ad_exit=&ad_boundf;
  arrmblsize = 4000000L;
  
  gradient_structure::set_NO_DERIVATIVES();
  gradient_structure::set_YES_SAVE_VARIABLES_VALUES();
  if (!arrmblsize) arrmblsize=15000000;

  model_parameters mp(arrmblsize,argc,argv);	// Step i) above
  mp.iprint=10;

  mp.preliminary_calculations();		// Step ii) above

  mp.computations(argc,argv);			// Step iii) above. Note that "mp" by inheritance
						// is also of class function_minimizer

  return 0;
}

/**
 * Function being called when the program has "walks out of an array" if the program has been compiled in "safe mode" (admb -s).
 * The purpose of the function is to allow the user to set a breakpoint for the debugger at a point just before the program terminates.
 * The reason for locating ad_boundf() in the model specific file is to give the user easy access to the function.
 * \param i the "exit status" set by the calling function.
 */
extern "C"  {
  void ad_boundf(int i)
  {
    /* so we can stop here */
    exit(i);
  }
}
