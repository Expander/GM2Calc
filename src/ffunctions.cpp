// ====================================================================
// This file is part of GM2Calc.
//
// GM2Calc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// GM2Calc is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GM2Calc.  If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

#include "ffunctions.hpp"
#include "dilog.h"
#include "numerics2.hpp"

#include <iostream>
#include <cmath>
#include <complex>

#define ERROR(message) std::cerr << "Error: " << message << '\n';

namespace gm2calc {

using namespace flexiblesusy;

const double precision = 1e-10;

double dilog(double x) {
   // The DiLogarithm function
   // Code translated by R.Brun from CERNLIB DILOG function C332

   const double PI = M_PI;
   const double HF  = 0.5;
   const double PI2 = PI*PI;
   const double PI3 = PI2/3;
   const double PI6 = PI2/6;
   const double PI12 = PI2/12;
   const double C[20] = {0.42996693560813697, 0.40975987533077105,
     -0.01858843665014592, 0.00145751084062268,-0.00014304184442340,
      0.00001588415541880,-0.00000190784959387, 0.00000024195180854,
     -0.00000003193341274, 0.00000000434545063,-0.00000000060578480,
      0.00000000008612098,-0.00000000001244332, 0.00000000000182256,
     -0.00000000000027007, 0.00000000000004042,-0.00000000000000610,
      0.00000000000000093,-0.00000000000000014, 0.00000000000000002};

   double T,H,Y,S,A,ALFA,B1,B2,B0;

   if (x == 1) {
       H = PI6;
   } else if (x == -1) {
       H = -PI12;
   } else {
       T = -x;
       if (T <= -2) {
           Y = -1/(1+T);
           S = 1;
           B1= log(-T);
           B2= log(1+1/T);
           A = -PI3+HF*(B1*B1-B2*B2);
       } else if (T < -1) {
           Y = -1-T;
           S = -1;
           A = log(-T);
           A = -PI6+A*(A+log(1+1/T));
       } else if (T <= -0.5) {
           Y = -(1+T)/T;
           S = 1;
           A = log(-T);
           A = -PI6+A*(-HF*A+log(1+T));
       } else if (T < 0) {
           Y = -T/(1+T);
           S = -1;
           B1= log(1+T);
           A = HF*B1*B1;
       } else if (T <= 1) {
           Y = T;
           S = 1;
           A = 0;
       } else {
           Y = 1/T;
           S = -1;
           B1= log(T);
           A = PI6+HF*B1*B1;
       }
       H    = Y+Y-1;
       ALFA = H+H;
       B1   = 0;
       B2   = 0;
       for (int i=19;i>=0;i--){
          B0 = C[i] + ALFA*B1-B2;
          B2 = B1;
          B1 = B0;
       }
       H = -(S*(B0-H*B2)+A);
    }
    return H;
}

std::complex<double> dilog(const std::complex<double>& x) {
   double a = x.real(), b = x.imag();
   double ansreal = 0., ansimag = 0.;

   dilogwrap_(&a, &b, &ansreal, &ansimag);

   return std::complex<double>(ansreal, ansimag);
}

double sqr(double x) { return x*x; }
double cube(double x) { return x*x*x; }
double quad(double x) { return sqr(x)*sqr(x); }
int sign(double x) { return x < 0 ? -1 : 1; }
double signed_sqr(double x) { return sign(x) * x * x; }
double signed_abs_sqrt(double x) {
   return sign(x) * std::sqrt(std::abs(x));
}

double F1C(double x) {
   if (is_equal(x, 1., 0.01)) {
      return 1. - 0.6*(-1 + x) + 0.4*sqr(-1 + x)
         - 0.2857142857142857*cube(-1 + x)
         + 0.21428571428571427*quad(-1 + x)
         - 0.16666666666666666*std::pow(-1 + x,5)
         + 0.13333333333333333*std::pow(-1 + x,6);
   }

   return 2. / quad(1. - x) * (2. + 3. * x - 6. * sqr(x)
                           + cube(x) + 6. * x * log(x));
}

double F2C(double x) {
   if (is_equal(x, 1., 0.01)) {
      return 1. - 0.75*(-1 + x) + 0.6*sqr(-1 + x) - 0.5*cube(-1 + x)
         + 0.42857142857142855*quad(-1 + x)
         - 0.375*std::pow(-1 + x,5)
         + 0.3333333333333333*std::pow(-1 + x,6);
   }

   return 3. / (2. * cube(1. - x)) * (- 3. + 4. * x - sqr(x) - 2. * log(x));
}

double F3C(double x) {
   if (is_equal(x, 1., 0.07)) {
      return 1. + 0.9012765957446807*(-1 + x)
         - 1.2235460992907807*sqr(-1 + x)
         + 1.2279808944854547*cube(-1 + x)
         - 1.1530221450282228*quad(-1 + x)
         + 1.0620712114633095*std::pow(-1 + x,5)
         - 0.9740314565542522*std::pow(-1 + x,6);
   }

   return ( 4. / (141. * quad(1. - x)) * ((1. - x) * (151. * sqr(x) - 335. * x + 592.)
             + 6. * (21. * cube(x) - 108. * sqr(x) - 93. * x + 50.) * log(x)
             - 54. * x * (sqr(x) - 2. * x - 2.) * sqr(log(x))
             - 108. * x * (sqr(x) - 2. * x + 12.) * dilog(1.- x)) );
}

double F4C(double x) {
   if (is_equal(x, 1., 0.07)) {
      return 1. - 0.3688524590163934*(-1 + x)
         + 0.15426229508196715*sqr(-1 + x)
         - 0.05573770491803279*cube(-1 + x)
         + 0.0037738374038140845*quad(-1 + x)
         + 0.025907494145199085*std::pow(-1 + x,5)
         - 0.04369818965837698*std::pow(-1 + x,6);
   }

   return ( - 9. / (122. * cube(1. - x)) * (8. * (sqr(x) - 3. * x + 2.)
             +  (11. * sqr(x) - 40. * x + 5.) * log(x)
             - 2. * (sqr(x) - 2. * x - 2.) * sqr(log(x))
             - 4. * (sqr(x) - 2. * x + 9.) * dilog(1.- x)) );
}

double F1N(double x) {
   if (is_equal(x, 1., 0.01)) {
      return 1. - 0.4*(-1 + x) + 0.2*sqr(-1 + x)
         - 0.1142857142857143*cube(-1 + x)
         + 0.07142857142857142*quad(-1 + x)
         - 0.047619047619047616*std::pow(-1 + x,5)
         + 0.03333333333333333*std::pow(-1 + x,6);
   }

   return 2. / quad(1. - x) * (1. - 6. * x + 3. * sqr(x)
                          + 2. * cube(x) - 6. * sqr(x) * log(x));
}

double F2N(double x) {
   if(is_equal(x, 1., 0.01)){
      return 1. - 0.5*(-1 + x) + 0.3*sqr(-1 + x)
         - 0.2*cube(-1 + x) + 0.14285714285714285*quad(-1 + x)
         - 0.10714285714285714*std::pow(-1 + x,5)
         + 0.08333333333333333*std::pow(-1 + x,6);
   }

   return 3. / cube(1. - x) * (1. - sqr(x) + 2. * x * log(x));
}

double F3N(double x) {
   if (is_equal(x, 1., 0.07)) {
      return 1. + 0.08685714285714365*(-1 + x)
         - 0.1641904761904751*sqr(-1 + x)
         + 0.13662973760932912*cube(-1 + x)
         - 0.1038192419825078*quad(-1 + x)
         + 0.07823129251700683*std::pow(-1 + x,5)
         - 0.05960544217687109*std::pow(-1 + x,6);
   }

   return 4. / (105. * quad(1. - x)) * ((1. - x) * (- 97. * sqr(x) - 529. * x + 2.)
            + 6. * sqr(x) * (13. * x + 81.) * log(x)
            + 108. * x * (7. * x + 4.) * dilog(1. - x));
}

double F4N(double x) {
   if (is_equal(x, 1., 0.07)) {
      return 1. + 6.245004513516506e-17*(-1 + x)
         - 0.13875*sqr(-1 + x)
         + 0.1475*cube(-1 + x)
         - 0.13163265306122446*quad(-1 + x)
         + 0.1128826530612245*std::pow(-1 + x,5)
         - 0.09610615079365077*std::pow(-1 + x,6);
   }

   return - 2.25 / cube(1. - x) * ((x + 3.) * (x * log(x) + x - 1.)
                                  + (6. * x + 2.) * dilog(1. - x));
}

/// Fb(x,1)
double Fb1(double x) {
   return (cube(x) - 6*sqr(x) + 3*x + 2. + 3*x*log(sqr(x)))
      / (6*quad(x - 1.));
}

/// Fb(x,x)
double Fbx(double x) {
   return 0.5 * (sqr(x) + 4*x - 5. - (2*x + 1.)*log(sqr(x)))
      / quad(x - 1.);
}

double Fb(double x, double y) {
   if (is_equal(x, 1., precision) && is_equal(y, 1., precision))
      return 1./12.;

   if (is_equal(x, 1., precision))
      return Fb1(y);

   if (is_equal(y, 1., precision))
      return Fb1(x);

   if (is_equal(x, y, precision))
      return Fbx(x);

   return - (G4(x) - G4(y)) / (x - y);
}

/// Fa(x,1)
double Fa1(double x) {
   return 0.25 * (cube(x) - 4*sqr(x) + 11*x - 8. - (x + 2.)*log(sqr(x)))
      / quad(x - 1.) + 0.5 * Fb1(x);
}

/// Fa(x,x)
double Fax(double x) {
   return 0.25 * (cube(x) - 16*sqr(x) + 11*x + 4. + x*(2*x + 7.)*log(sqr(x)))
      / (x * quad(x - 1.)) + 0.5 * Fbx(x);
}

double Fa(double x, double y) {
   if (is_equal(x, 1., precision) && is_equal(y, 1., precision))
      return 0.25;

   if (is_equal(x, 1., precision))
      return Fa1(y);

   if (is_equal(y, 1., precision))
      return Fa1(x);

   if (is_equal(x, y, precision))
      return Fax(x);

   return - (G3(x) - G3(y)) / (x - y);
}

double G3(double x) {
   if(is_equal(x, 1.))
      ERROR("G3: x must not be 1 !");

   return 1. / (2. * cube(x - 1.)) * ((x - 1.) * (x - 3.) + 2. * log(x));
}

double G4(double x) {
   if(is_equal(x, 1.))
      ERROR("G4: x must not be 1 !");

   return 1. / (2. * cube(x - 1.)) * ((x - 1.) * (x + 1.) - 2. * x * log(x));
}

/**
 * \f$H_2(x,y)\f$ function from Eq. (34) of arxiv:0901.2065 .
 *
 * The \f$H_2(x,y)\f$ function is related to \f$I(a,b,c)\f$ by
 * \f$\frac{1}{z} H_2(\frac{x}{z},\frac{y}{z}) = - I(x,y,z)\f$.
 */
double H2(double x, double y) {
   if (is_equal(x, 1., precision) && is_equal(y, 1., precision))
      return -0.5;

   if (is_equal(x, 1., precision))
      return (-1. + y - y*log(y))/sqr(-1. + y);

   if (is_equal(y, 1., precision))
      return (-1. + x - x*log(x))/sqr(-1. + x);

   if (is_equal(x, y, precision))
      return (1 - y + log(y))/sqr(-1 + y);

   return x * log(x) / ((1-x)*(x-y)) + y * log(y) / ((1-y)*(y-x));
}

/// Iabc(a,a,c)
double Iaac(double a, double c) {
   return (sqr(a) - sqr(c) - sqr(c)*log(sqr(a/c))) / sqr(sqr(a) - sqr(c));
}

double Iabc(double a, double b, double c) {
   if (is_equal(a,b) && is_equal(b,c))
      return 0.5 / sqr(a);

   if (is_equal(a,b))
      return Iaac(a,c);

   if (is_equal(b,c))
      return Iaac(b,a);

   if (is_equal(a,c))
      return Iaac(a,b);

   return ( (sqr(a * b) * log(sqr(a / b))
           + sqr(b * c) * log(sqr(b / c))
           + sqr(c * a) * log(sqr(c / a)))
           / ((sqr(a) - sqr(b)) * (sqr(b) - sqr(c)) * (sqr(a) - sqr(c))) );
}

/**
 * Calculates \f$f_{PS}(z)\f$, Eq. (70) arXiv:hep-ph/0609168
 */
double f_PS(double z) {
   double result = 0.;
   if(z < 0.25) {
      double y = sqrt(1. - 4. * z);
      result = 2. * z / y * (dilog(1. - 0.5 * (1. - y) / z) - dilog(1. - 0.5 * (1. + y) / z));
   } else {
      std::complex<double> y = sqrt(std::complex<double>(1. - 4. * z, 0.));
      std::complex<double> zc(z, 0.);
      result = real(2. * zc / y * (dilog(1. - 0.5 * (1. - y) / zc) - dilog(1. - 0.5 * (1. + y) / zc)));
   }

   return result;
}

/**
 * Calculates \f$f_S(z)\f$, Eq. (71) arXiv:hep-ph/0609168
 */
double f_S(double z) {
   if(z < 0.)
      ERROR("f_S: z must not be negativ!");

   return (2. * z - 1.) * f_PS(z) - 2. * z * (2. + log(z));
}

/**
 * Calculates \f$f_{\tilde{f}}(z)\f$, Eq. (72) arXiv:hep-ph/0609168
 */
double f_sferm(double z) {
   if(z < 0.)
      ERROR("f_sferm: z must not be negativ!");

   return 0.5 * z * (2. + log(z) - f_PS(z));
}

} // namespace gm2calc
