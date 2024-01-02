/*

-Procedure saelgv_c ( Semi-axes of ellipse from generating vectors )

-Abstract

   Find semi-axis vectors of an ellipse generated by two arbitrary
   three-dimensional vectors.

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

   ELLIPSES

-Keywords

   ELLIPSE
   GEOMETRY
   MATH

*/

   #include "SpiceUsr.h"
   #include "SpiceZfc.h"
   #include "SpiceZmc.h"
   #undef   saelgv_c


   void saelgv_c ( ConstSpiceDouble   vec1  [3],
                   ConstSpiceDouble   vec2  [3],
                   SpiceDouble        smajor[3],
                   SpiceDouble        sminor[3]  )

/*

-Brief_I/O

   VARIABLE  I/O  DESCRIPTION
   --------  ---  --------------------------------------------------
   vec1,
   vec2       I   Two vectors used to generate an ellipse.
   smajor     O   Semi-major axis of ellipse.
   sminor     O   Semi-minor axis of ellipse.

-Detailed_Input

   vec1,
   vec2        are two vectors that define an ellipse.
               The ellipse is the set of points in 3-space

                  center  +  cos(theta) vec1  +  sin(theta) vec2

               where theta is in the interval ( -pi, pi ] and
               `center' is an arbitrary point at which the ellipse
               is centered. An ellipse's semi-axes are
               independent of its center, so the vector `center'
               shown above is not an input to this routine.

               `vec2' and `vec1' need not be linearly independent;
               degenerate input ellipses are allowed.

-Detailed_Output

   smajor,
   sminor      are semi-major and semi-minor axes of the ellipse,
               respectively. `smajor' and `sminor' may overwrite
               either of `vec1' or `vec2'.

-Parameters

   None.

-Exceptions

   1)  If one or more semi-axes of the ellipse is found to be the
       zero vector, the input ellipse is degenerate. This case is
       not treated as an error; the calling program must determine
       whether the semi-axes are suitable for the program's intended
       use.

-Files

   None.

-Particulars

   Two linearly independent but not necessarily orthogonal vectors
   `vec1' and `vec2' can define an ellipse centered at the origin: the
   ellipse is the set of points in 3-space

      center  +  cos(theta) vec1  +  sin(theta) vec2

   where theta is in the interval (-pi, pi] and `center' is an
   arbitrary point at which the ellipse is centered.

   This routine finds vectors that constitute semi-axes of an
   ellipse that is defined, except for the location of its center,
   by `vec1' and `vec2'. The semi-major axis is a vector of largest
   possible magnitude in the set

      cos(theta) vec1  +  sin(theta) vec2

   There are two such vectors; they are additive inverses of each
   other. The semi-minor axis is an analogous vector of smallest
   possible magnitude. The semi-major and semi-minor axes are
   orthogonal to each other. If `smajor' and `sminor' are choices of
   semi-major and semi-minor axes, then the input ellipse can also
   be represented as the set of points

      center  +  cos(theta) smajor  +  sin(theta) sminor

   where theta is in the interval (-pi, pi].

   The capability of finding the axes of an ellipse is useful in
   finding the image of an ellipse under a linear transformation.
   Finding this image is useful for determining the orthogonal and
   gnomonic projections of an ellipse, and also for finding the limb
   and terminator of an ellipsoidal body.

-Examples

   1)  An example using inputs that can be readily checked by
       hand calculation.

          Let

             vec1 = ( 1.,  1.,  1. )
             vec2 = ( 1., -1.,  1. )

         The function call

            saelgv_c ( vec1, vec2, smajor, sminor );

         returns

            smajor = ( -1.414213562373095,
                        0.0,
                       -1.414213562373095 )
         and

            sminor = ( -2.4037033579794549D-17
                        1.414213562373095,
                       -2.4037033579794549D-17 )


   2)   This example is taken from the code of the CSPICE routine
        pjelpl_c, which finds the orthogonal projection of an ellipse
        onto a plane. The code listed below is the portion used to
        find the semi-axes of the projected ellipse.


           #include "SpiceUsr.h"
                 .
                 .
                 .

           /.
           Project vectors defining axes of ellipse onto plane.
           ./
           vperp_c ( vec1,   normal,  proj1 );
           vperp_c ( vec2,   normal,  proj2 );

              .
              .
              .

           saelgv_c ( proj1,  proj2,  smajor,  sminor );


        The call to saelgv_c determines the required semi-axes.

-Restrictions

   None.

-Literature_References

   [1]  T. Apostol, "Calculus, Vol. II," chapter 5, "Eigenvalues of
        Operators Acting on Euclidean Spaces," John Wiley & Sons,
        1969.

-Author_and_Institution

   N.J. Bachman        (JPL)
   J. Diaz del Rio     (ODC Space)
   W.L. Taber          (JPL)

-Version

   -CSPICE Version 1.0.1, 13-APR-2021 (JDR)

       Edited the header to comply with NAIF standard.

   -CSPICE Version 1.0.0, 12-JUN-1999 (NJB) (WLT)

-Index_Entries

   semi-axes of ellipse from generating vectors

-&
*/

{ /* Begin saelgv_c */


   /*
   Local macros
   */
   #define CLEAR_VEC( v )    (v)[0] = 0.; (v)[1] = 0.; (v)[2] = 0.;


   /*
   Local variables
   */
   SpiceDouble             c       [2][2];
   SpiceDouble             eigval  [2][2];
   SpiceDouble             s       [2][2];
   SpiceDouble             scale;
   SpiceDouble             tmpvc1  [3];
   SpiceDouble             tmpvc2  [3];

   SpiceInt                i;
   SpiceInt                major;
   SpiceInt                minor;




   /*
   Static variables
   */


   /*
   Participate in error tracing.
   */

   chkin_c ( "saelgv_c" );



   /*
   Let the notation

      < a, b >

   indicate the inner product of the vectors a and b.

   The semi-major and semi-minor axes of the input ellipse are
   vectors of maximum and minimum norm in the set

      cos(x) vec1  +  sin(x) vec2

   where x is in the interval (-pi, pi].

   The square of the norm of a vector in this set is

                                           2
          || cos(x) vec1  +  sin(x) vec2 ||


      =   < cos(x)vec1 + sin(x)vec2,  cos(x)vec1 + sin(x)vec2 > ;

   this last expression can be written as the matrix product

       T
      X  S  X,                                                 (1)

   where X is the unit vector

      +-      -+
      | cos(x) |
      |        |
      | sin(x) |
      +-      -+

   and S is the symmetric matrix

      +-                                -+
      | < vec1, vec1 >    < vec1, vec2 > |
      |                                  |.
      | < vec1, vec2 >    < vec2, vec2 > |
      +-                                -+

   Because the 2x2 matrix above is symmetric, there exists a
   rotation matrix that allows us to diagonalize it:

       T
      C  S  C  =  D,

   where D is a diagonal matrix.  Since rotation matrices are
   orthogonal, we have

       T
      C  C  =  I.

   If the unit vector U is defined by

           T
      U = C X,

   then

       T             T  T         T               T
      X  S  X  =  ( U  C  )  C D C   ( C U )  =  U  D  U.

   So, letting

      +-   -+
      |  u  |
      |     |  =  U,
      |  v  |
      +-   -+

   we may re-write the original quadratic expression (1) as

      +-     -+    +-        -+    +-   -+
      | u   v |    | D1    0  |    |  u  |,
      +-     -+    |          |    |     |
                   |          |    |  v  |
                   | 0     D2 |    +-   -+
                   +-        -+
   or

          2            2
      D1 u    +    D2 v,

   where the diagonal matrix above is D.  The eigenvalues D1 and
   D2 are non-negative because they are eigenvalues of a positive
   semi-definite matrix of the form

       T
      M  M.

   We may require that

      D1  >  D2;
          -

   then the maximum and minimum values of

          2            2
      D1 u    +    D2 v                                        (2)

   are D1 and D2 respectively.  These values are the squares
   of the lengths of the semi-major and semi-minor axes of the
   ellipse, since the expression (2) is the square of the norm
   of the point

      cos(x) vec1  + sin(x) vec2.

   Now we must find some eigenvectors.  Since the extrema of (2)
   occur when

           +-   -+                     +-   -+
           |  1  |                     |  0  |
      U =  |     |       or       U =  |     |,
           |  0  |                     |  1  |
           +-   -+                     +-   -+

   and since

      X = C U,

   we conclude that the extrema occur when X = C1 or X = C2, where
   C1 and C2 are the first and second columns of C.  Looking at
   the definition of X, we see that the extrema occur when

      cos(x) = C1[0]
      sin(x) = C1[1]

   and when

      cos(x) = C2[0],
      sin(x) = C2[1]

   So the semi-major and semi-minor axes of the ellipse are

      C[0][0] vec1  +  C[1][0] vec2

   and

      C[0][1] vec1  +  C[1][1] vec2

   (the negatives of these vectors are also semi-axes).

   */



   /*
   Copy the input vectors.
   */
   MOVED ( vec1, 3, tmpvc1 );
   MOVED ( vec2, 3, tmpvc2 );


   /*
   Scale the vectors to try to prevent arithmetic unpleasantness.
   We avoid using the quotient 1/SCALE, as this value may overflow.
   No need to go further if SCALE turns out to be zero.
   */
   scale = MaxAbs (  vnorm_c ( tmpvc1 ),   vnorm_c ( tmpvc2 )  );

   if ( scale == 0.0 )
   {
      CLEAR_VEC ( smajor );
      CLEAR_VEC ( sminor );

      chkout_c ( "saelgv_c" );
      return;
   }

   for ( i = 0;  i < 3;  i++ )
   {
      tmpvc1[i] = tmpvc1[i]/scale;
      tmpvc2[i] = tmpvc2[i]/scale;
   }


   /*
   Compute S and diagonalize it:
   */
   s[0][0]  =  vdot_c ( tmpvc1, tmpvc1 );
   s[1][0]  =  vdot_c ( tmpvc1, tmpvc2 );
   s[0][1]  =  s[1][0];
   s[1][1]  =  vdot_c ( tmpvc2, tmpvc2 );


   diags2_c ( s, eigval, c );


   /*
   Find the semi-axes.
   */

   if (   fabs( eigval[0][0] )  >=  fabs( eigval[1][1] )   )
   {
      /*
      The first eigenvector ( first column of C ) corresponds
      to the semi-major axis of the ellipse.
      */
      major = 0;
      minor = 1;
   }
   else
   {
      /*
      The second eigenvector corresponds to the semi-major axis.
      */
      major = 1;
      minor = 0;
   }


   vlcom_c ( c[0][major], tmpvc1, c[1][major], tmpvc2, smajor );
   vlcom_c ( c[0][minor], tmpvc1, c[1][minor], tmpvc2, sminor );


   /*
   Undo the initial scaling.
   */
   vscl_c ( scale, smajor, smajor );
   vscl_c ( scale, sminor, sminor );


   chkout_c ( "saelgv_c" );

} /* End saelgv_c */
