/*
 * $Id$
 *
 * Author: David Fournier
 * Copyright (c) 2008-2014 Regents of the University of California
 *
 * ADModelbuilder and associated libraries and documentations are
 * provided under the general terms of the "New BSD" license
 *
 * License:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2.  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3.  Neither the name of the  University of California, Otter Research,
 * nor the ADMB Foundation nor the names of its contributors may be used
 * to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __ADMB_GRADIENT_STRUCTURE_H__
#define __ADMB_GRADIENT_STRUCTURE_H__

#include <fstream>
using std::ofstream;

class dvariable;
class DF_FILE;
class dvector;
class dmatrix;
class dlink;
class double_and_int;
class arr_list;
class dvar_vector;
class dvar_vector_position;
class prevariable;
class indvar_offset_list;
class dependent_variables_information;
class grad_stack;
class uostream;
class dlist;

/**
 * Description not yet available.
 * \param
 */
class humungous_pointer
{
  size_t adjustment;
public:
  humungous_pointer();
  ~humungous_pointer() {}

  char* ptr;
  void free();
  void adjust(const size_t);
  humungous_pointer operator+(unsigned long int& offset);
  humungous_pointer& operator+=(unsigned long int& offset);
  humungous_pointer& operator=(void* p);
  int operator==(void* p);
  int operator!=(void* p);
  operator char*();
  operator void*();
  operator double_and_int*();
  operator double*();
};

/**
 * class for things related to the gradient structures, including dimension of
 * arrays, size of buffers, etc.
 */
class gradient_structure
{
   static char cmpdif_file_name[61];
   static DF_FILE *fp;
 public:
#if defined(NO_DERIVS)
   static int no_derivatives;
#endif
 private:
   static long int USE_FOR_HESSIAN;
   static long int NVAR;
   static int NUM_RETURN_ARRAYS;
   static dvariable **RETURN_ARRAYS;
   static int RETURN_ARRAYS_PTR;
   static dvariable **RETURN_PTR_CONTAINER;
   static size_t TOTAL_BYTES;
   static size_t PREVIOUS_TOTAL_BYTES;
   static unsigned long ARRAY_MEMBLOCK_SIZE;//js
   static humungous_pointer ARRAY_MEMBLOCK_BASE;
   static humungous_pointer ARRAY_MEMBLOCK_SAVE;
 public:
   static double *get_ARRAY_MEMBLOCK_BASE()
   {
      return (double*)ARRAY_MEMBLOCK_BASE;
   }
 private:
#ifdef __BORLANDC__
   static long int CMPDIF_BUFFER_SIZE;
   static long int GRADSTACK_BUFFER_SIZE;
#else
   static size_t CMPDIF_BUFFER_SIZE;
   static size_t GRADSTACK_BUFFER_SIZE;
#endif
   static unsigned int MAX_NVAR_OFFSET;
   static int save_var_file_flag;
   static int save_var_flag;

   static unsigned int MAX_DLINKS;
   static indvar_offset_list *INDVAR_LIST;
   static int NUM_DEPENDENT_VARIABLES;
   static dependent_variables_information *DEPVARS_INFO;

   // this needs to be a static member function so other static
   // member functions can call it
   static void check_set_error(const char *variable_name);

   static int instances;
   int x;

 public:
   // exception class
   class arrmemblerr
   {
   };

   static int Hybrid_bounded_flag;
   static double *hessian_ptr;
   static long int get_USE_FOR_HESSIAN()
   {
      return USE_FOR_HESSIAN;
   }
   static void set_USE_FOR_HESSIAN(const long int i)
   {
      USE_FOR_HESSIAN = i;
   }
   friend class dfsdmat;
   gradient_structure(long int size = 100000L);// constructor
   ~gradient_structure(void);// destructor
   static void save_variables(void);
   static void restore_variables(void);
   static void save_arrays(void);
   static void restore_arrays(void);
   static size_t totalbytes(void);
   friend dvector restore_dvar_vector_value(
     const dvar_vector_position& tmp);
   friend void cleanup_temporary_files();
   //friend dvector restore_dvar_vector_value(const dvar_vector_position&);
   friend dvector restore_dvar_vector_derivatives(void);
   friend dmatrix restore_dvar_matrix_derivatives(void);
   friend dmatrix restore_dvar_matrix_value(void);
   //friend dmatrix restore_derivatives(void);
   friend void gradfree(dlink * v);
   friend double_and_int *arr_new(unsigned int sz);//js
   friend void arr_free(double_and_int *);
   friend void RETURN_ARRAYS_DECREMENT(void);
   friend void RETURN_ARRAYS_INCREMENT(void);
   friend void make_indvar_list(const dvar_vector & t);
   //friend void gradcalc( int , double *);
   friend void gradcalc(int nvar, const dvector & g);
   friend void slave_gradcalc(void);
   friend void funnel_gradcalc(void);
   friend void allocate_dvariable_space(void);
   friend void wide_funnel_gradcalc(void);
   friend dvar_vector_position restore_dvar_vector_position(void);
   static grad_stack *GRAD_STACK1;
   friend double_and_int *gradnew();
   static dlist *GRAD_LIST;
   static int RETURN_ARRAYS_SIZE;
   //static int RETURN_INDEX;
   static dvariable *RETURN_PTR;
   static dvariable *MIN_RETURN;
   static dvariable *MAX_RETURN;
   static arr_list *ARR_LIST1;
   static arr_list *ARR_FREE_LIST1;
   //static void funnel_jacobcalc(void);
   static void jacobcalc(int nvar, const dmatrix & jac);
   static void jacobcalc(int nvar, const ofstream & jac);
   static void jacobcalc(int nvar, const uostream & jac);

   friend void default_evaluation(void);
   //access functions

   friend class DF_FILE;
   static DF_FILE *get_fp(void)
   {
      return fp;
   }
   static void set_NUM_RETURN_ARRAYS(int i);
#if defined(NO_DERIVS)
   static void set_NO_DERIVATIVES(void);
   static void set_YES_DERIVATIVES(void);
#endif
   static void set_YES_SAVE_VARIABLES_VALUES();
   static void set_NO_SAVE_VARIABLES_VALUES();
   //static int _GRADFILE_PTR; // should be int gradfile_handle;
   //static int _GRADFILE_PTR1; // should be int gradfile_handle;
   //static int _GRADFILE_PTR2; // should be int gradfile_handle;
   //static int _VARSSAV_PTR; // should be int gradfile_handle;
   static void set_NUM_DEPENDENT_VARIABLES(int i);
   static void set_RETURN_ARRAYS_SIZE(int i);
   static void set_ARRAY_MEMBLOCK_SIZE(unsigned long i);
#ifdef __BORLANDC__
   static void set_CMPDIF_BUFFER_SIZE(long int i);
   static void set_GRADSTACK_BUFFER_SIZE(long int i);
   static void set_GRADSTACK_BUFFER_BYTES(long int i);
#else
   static void set_CMPDIF_BUFFER_SIZE(const size_t i);
   static void set_GRADSTACK_BUFFER_SIZE(const size_t i);
   static void set_GRADSTACK_BUFFER_BYTES(const size_t i);
#endif
   static void set_MAX_NVAR_OFFSET(unsigned int i);
   static void set_MAX_DLINKS(int i);
   static size_t NUM_GRADSTACK_BYTES_WRITTEN(void);
   friend class dlist;
   friend class grad_stack;
   static void save_dependent_variable_position(const prevariable & v1);
   static unsigned long int max_last_offset;
   friend class function_minimizer;
   friend void funnel_derivatives(void);
};

/**
 * Description not yet available.
 * \param
 */
class DF_FILE
{
public:
  DF_FILE(const size_t nbytes);
  ~DF_FILE();

  char* buff;
  size_t toffset;
  union
  {
    size_t offset;
    char fourb[sizeof(size_t)];
  };
  char cmpdif_file_name[81];
  int file_ptr;

  void fwrite(const void *s, const size_t num_bytes);
  void fread(void *s, const size_t num_bytes);

  void fwrite(const int &);
  void fread(const int &);

  void fwrite(double);
  void fread(const double &);

  void fread(void *&ptr);
  void fwrite(void *ptr);

  void write_cmpdif_stack_buffer(void);
  void read_cmpdif_stack_buffer(off_t & lpos);

private:
#ifdef _MSC_VER
  size_t buff_end;
  size_t buff_size;
#else
  const size_t buff_end;
  const size_t buff_size;
#endif
};
#endif
