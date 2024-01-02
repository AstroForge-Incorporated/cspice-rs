/*

-Procedure mtxmg_c ( Matrix transpose times matrix, general dimension )

-Abstract

   Multiply the transpose of a matrix with another matrix,
   both of arbitrary size. (The dimensions of the matrices must be
   compatible with this multiplication.)

-Disclaimer

   THIS SOFTWARE AND ANY RELATED MATERIALS WERE CREATED BY THE
   CALIFORNIA INSTITUTE OF TECHNOLOGY (CALTECH) UNDER A U.S.
   GOVERNMENT CONTRACT WITH THE NATIONAL AERONAUTICS AND SPACE
   ADMINISTRATION (NASA). THE SOFTWARE IS TECHNOLOGY AND SOFTWARE
   PUBLICLY AVAILABLE UNDER U.S. EXPORT LAWS AND IS PROVIDED "AS-IS"
   TO THE RECIPIENT WITHOUT WARRANTY OF ANY KIND, INCLUDING ANY
   WARRANTIES OF PERFORMANCE OR MERCHANTABILITY OR FITNESS FOR A
   PARTICULAR USE OR PURPOSE (AS SET FORTH IN UNITED STATES UCC
   SECTIONS 2312-2313) OR FOR ANY PURPOSE WHATSOEVER, FOR THE
   SOFTWARE AND RELATED MATERIALS, HOWEVER USED.

   IN NO EVENT SHALL CALTECH, ITS JET PROPULSION LABORATORY, OR NASA
   BE LIABLE FOR ANY DAMAGES AND/OR COSTS, INCLUDING, BUT NOT
   LIMITED TO, INCIDENTAL OR CONSEQUENTIAL DAMAGES OF ANY KIND,
   INCLUDING ECONOMIC DAMAGE OR INJURY TO PROPERTY AND LOST PROFITS,
   REGARDLESS OF WHETHER CALTECH, JPL, OR NASA BE ADVISED, HAVE
   REASON TO KNOW, OR, IN FACT, SHALL KNOW OF THE POSSIBILITY.

   RECIPIENT BEARS ALL RISK RELATING TO QUALITY AND PERFORMANCE OF
   THE SOFTWARE AND ANY RELATED MATERIALS, AND AGREES TO INDEMNIFY
   CALTECH AND NASA FOR ALL THIRD-PARTY CLAIMS RESULTING FROM THE
   ACTIONS OF RECIPIENT IN THE USE OF THE SOFTWARE.

-Required_Reading

   None.

-Keywords

   MATRIX

*/

   #include <stdlib.h>
   #include <math.h>
   #include "SpiceUsr.h"
   #include "SpiceZmc.h"
   #include "SpiceZim.h"
   #undef    mtxmg_c

   void mtxmg_c ( const void  * m1,
                  const void  * m2,
                  SpiceInt      nc1,
                  SpiceInt      nr1r2,
                  SpiceInt      nc2,
                  void        * mout  )

/*

-Brief_I/O

   VARIABLE  I/O  DESCRIPTION
   --------  ---  --------------------------------------------------
   m1         I   nr1r2 X nc1 double precision matrix.
   m2         I   nr1r2 X nc2 double precision matrix.
   nc1        I   Column dimension of `m1' and row dimension of `mout'.
   nr1r2      I   Row dimension of `m1' and `m2'.
   nc2        I   Column dimension of `m2' (and also `mout').
   mout       O   Transpose of `m1' times `m2'.

-Detailed_Input

   m1          is any double precision matrix of arbitrary size.

   m2          is any double precision matrix of arbitrary size.
               The number of rows in `m2' must match the number of
               rows in `m1'.

   nc1         is the number of columns in `m1' and the number of rows
               of `mout'.

   nr1r2       is the number of rows in both `m1' and (by necessity)
               `m2'.

   nc2         is the number of columns in both `m2' and `mout'.

-Detailed_Output

   mout        is the product matrix defined as the transpose of
               `m1' times `m2', that is

                             t
                  mout = (m1)  x (m2)

               where the superscript `t' denotes the transpose of `m1'.

               `mout' is a double precision matrix of dimension nc1 x
               nc2.

               `mout' may overwrite `m1' or `m2'.

-Parameters

   None.

-Exceptions

   1)  If nr1r2 < 0, the elements of the matrix `mout' are set equal to
       zero.

   2)  If memory cannot be allocated to create the temporary matrix
       required for the execution of the routine, the error
       SPICE(MALLOCFAILED) is signaled.

-Files

   None.

-Particulars

   The code reflects precisely the following mathematical expression

   For each value of the subscript `i' from 1 to `nc1', and `j' from 1
   to `nc2':

      mout(i,j) = Summation from k=1 to nr1r2 of  m1(k,i) * m2(k,j)

-Examples

   The numerical results shown for this example may differ across
   platforms. The results depend on the SPICE kernels used as
   input, the compiler and supporting libraries, and the machine
   specific arithmetic implementation.

   1) Given a 2x4 and a 2x3 matrices, multiply the transpose of the
      first matrix by the second one.


      Example code begins here.


      /.
         Program mtxmg_ex1
      ./
      #include <stdio.h>
      #include "SpiceUsr.h"

      int main( )
      {

         /.
         Local variables.
         ./
         SpiceDouble          mout   [4][3];

         SpiceInt             i;

         /.
         Define `m1' and `m2'.
         ./
         SpiceDouble          m1     [2][4] = { { 1.0, 2.0, 3.0, 0.0 },
                                                { 1.0, 1.0, 1.0, 0.0 }  };

         SpiceDouble          m2     [2][3] = { { 1.0, 2.0, 3.0 },
                                                { 0.0, 0.0, 0.0 } };

         /.
         Multiply the transpose of `m1' by `m2'.
         ./
         mtxmg_c ( m1, m2, 4, 2, 3, mout );

         printf( "Transpose of M1 times M2:\n" );
         for ( i = 0; i < 4; i++ )
         {

            printf( "%10.3f %9.3f %9.3f\n",
                    mout[i][0], mout[i][1], mout[i][2] );

         }

         return ( 0 );
      }


      When this program was executed on a Mac/Intel/cc/64-bit
      platform, the output was:


      Transpose of M1 times M2:
           1.000     2.000     3.000
           2.000     4.000     6.000
           3.000     6.000     9.000
           0.000     0.000     0.000


-Restrictions

   1)  No error checking is performed to prevent numeric overflow or
       underflow.

   2)  No error checking performed to determine if the input and
       output matrices have, in fact, been correctly dimensioned.

-Literature_References

   None.

-Author_and_Institution

   N.J. Bachman        (JPL)
   J. Diaz del Rio     (ODC Space)
   W.M. Owen           (JPL)
   E.D. Wright         (JPL)

-Version

   -CSPICE Version 1.3.0, 10-AUG-2021 (JDR)

       Changed the input argument names "ncol1" and "ncol2" to "nc1"
       and "nc2" for consistency with other routines.

       Updated short error message for consistency within CSPICE wrapper
       interface: MEMALLOCFAILED -> MALLOCFAILED.

       Edited the header to comply with NAIF standard.
       Added complete code example based on the existing example.

       Added entry #1 to -Exceptions section.

   -CSPICE Version 1.2.2, 16-JAN-2008 (EDW)

       Corrected typos in header titles:

       Detailed Input to -Detailed_Input
       Detailed Output to -Detailed_Output

   -CSPICE Version 1.2.1, 10-NOV-2006 (EDW)

       Added -Parameters section header.

   -CSPICE Version 1.2.0, 28-AUG-2001 (NJB)

       Const-qualified input arrays.

   -CSPICE Version 1.0.0, 16-APR-1999 (NJB) (WMO)

-Index_Entries

   matrix transpose times matrix n-dimensional_case

-&
*/

{  /* Begin mtxmg_c */


   /*
   Local macros

   We'd like to be able to refer to the elements of the input and output
   matrices using normal subscripts, for example, m1[2][3].  Since the
   compiler doesn't know how to compute index offsets for the array
   arguments, which have user-adjustable size, we must compute the
   offsets ourselves.  To make syntax a little easier to read (we hope),
   we'll use macros to do the computations.

   The macro INDEX(width, i,j) computes the index offset from the array
   base of the element at position [i][j] in a 2-dimensional matrix
   having the number of columns indicated by width.  For example, if
   the input matrix m1 has 2 rows and 3 columns, the element at position
   [0][1] would be indicated by

      m1[ INDEX(3,0,1) ]

   */

   #define INDEX( width, row, col )     ( (row)*(width) + (col) )


   /*
   Local variables
   */
   SpiceDouble             innerProduct;
   SpiceDouble            *tmpmat;
   SpiceDouble            *loc_m1;
   SpiceDouble            *loc_m2;

   SpiceInt                col;
   SpiceInt                i;
   SpiceInt                nelt;
   SpiceInt                row;

   size_t                  size;


   /*
   Allocate space for a temporary copy of the output matrix, which
   has nc1 rows and nc2 columns.
   */
   nelt   =  nc1 * nc2;
   size   =  (size_t) ( nelt * sizeof(SpiceDouble) );

   tmpmat =  (SpiceDouble *) malloc ( size );

   if ( tmpmat == (SpiceDouble *)0 )
   {
      chkin_c  ( "mtxmg_c"                                         );
      setmsg_c ( "An attempt to create a temporary matrix failed." );
      sigerr_c ( "SPICE(MALLOCFAILED)"                             );
      chkout_c ( "mtxmg_c"                                         );
      return;
   }

   /*
   Cast the input pointers to pointers to SpiceDoubles.  Note:  the
   original variables are pointers to void so that callers may
   supply the array names as arguments without casting them to
   SpiceDoubles.  The naked array name is considered by the compiler
   to be an incompatible pointer type with (SpiceDouble *), so we
   can't simply declare the arguments to be (SpiceDouble *).  On the
   other hand, every pointer type can be cast to (void *).
   */

   loc_m1 = (SpiceDouble *) m1;
   loc_m2 = (SpiceDouble *) m2;


   /*
   Compute the product.  The matrix element at position (row,col) is
   the inner product of the column of m1 having index row and the
   column of m2 having index col.  We compute index offsets using
   the macro INDEX.
   */

   for ( row = 0;  row < nc1;  row++ )
   {

      for ( col = 0;  col < nc2;  col++ )
      {
         innerProduct = 0.0;

         for ( i = 0;  i < nr1r2;  i++ )
         {
            innerProduct +=   loc_m1[ INDEX(nc1, i, row) ]
                            * loc_m2[ INDEX(nc2, i, col) ];
         }

         tmpmat [ INDEX( nc2, row, col ) ]  =  innerProduct;
      }
   }

   /*
   Move the result from tmpmat into mout.
   */
   MOVED ( tmpmat, nelt, mout );

   /*
   Free the temporary matrix.
   */
   free ( tmpmat );


} /* End mtxmg_c */
