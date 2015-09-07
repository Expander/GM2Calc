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

#include "dilog.hpp"
#include <cmath>
#include <limits>

namespace gm2calc {

namespace {
   template <typename T>
   T sqr(T x) { return x*x; }
}

/**
 * @brief Real dilogarithm \f$\mathrm{Li}_2(z)\f$
 * @param z real argument
 * @note Implementation translated by R.Brun from CERNLIB DILOG function C332
 * @return \f$\mathrm{Li}_2(z)\f$
 */
double dilog(double x) {
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

/**
 * @brief Complex dilogarithm \f$\mathrm{Li}_2(z)\f$
 * @param z complex argument
 * @note Implementation translated from SPheno to C++
 * @return \f$\mathrm{Li}_2(z)\f$
 */
std::complex<double> dilog(const std::complex<double>& z) {
   std::complex<double> cy, cz;
   int jsgn, ipi12;
   static const unsigned N = 20;

   // Bernulli numbers / (2 n + 1)!
   const double bf[N] = {
      - 1./4.,
      + 1./36.,
      - 1./36.e2,
      + 1./21168.e1,
      - 1./108864.e2
      + 1./52690176.e1,
      - 691./16999766784.e3,
      + 1./1120863744.e3,
      - 3617./18140058832896.e4,
      + 43867./97072790126247936.e3,
      - 174611./168600109166641152.e5,
      + 77683./32432530090601152512.e4,
      - 236364091./4234560341829359173632.e7,
      + 657931./5025632054039239458816.e6,
      - 3392780147./109890470493622010006470656.e7,
      + 172.3168255201/2355349904102724211909.3102313472e6,
      - 770.9321041217/4428491985594062112714.2791446528e8,
      + 4.1576356446138997196178996207752266734882541595116e-29,
      - 9.9621484882846221031940067024558388498548600173945e-31,
      + 2.3940344248961653005211679878937495629342791569329e-32
   };

   const double rz = std::real(z);
   const double iz = std::imag(z);
   const double az = std::sqrt(sqr(rz) + sqr(iz));

   // special cases
   if (iz == 0.) {
      if (rz <= 1.)
         return std::complex<double>(dilog(rz), 0.);
      if (rz > 1.)
         return std::complex<double>(dilog(rz), -M_PI*std::log(rz));
   } else if (az < std::numeric_limits<double>::epsilon()) {
      return z;
   }

   // transformation to |z|<1, Re(z)<=0.5
   if (rz <= 0.5) {
      if (az > 1.) {
         cy = -0.5 * sqr(std::log(-z));
         cz = -std::log(1. - 1. / z);
         jsgn = -1;
         ipi12 = -2;
      } else { // (az <= 1.)
         cy = 0;
         cz = -std::log(1. - z);
         jsgn = 1;
         ipi12 = 0;
      }
   } else { // rz > 0.5
      if (az <= std::sqrt(2*rz)) {
         cz = -std::log(z);
         cy = cz * std::log(1. - z);
         jsgn = -1;
         ipi12 = 2;
      } else { // (az > sqrt(2*rz))
         cy = -0.5 * sqr(std::log(-z));
         cz = -std::log(1. - 1. / z);
         jsgn = -1;
         ipi12 = -2;
      }
   }

   // the dilogarithm
   const std::complex<double> cz2(sqr(cz));
   std::complex<double> sumC;

   for (unsigned i1 = 2; i1 < N; i1++)
      sumC = cz2 * (sumC + bf[N + 1 - i1]);

   // lowest order terms w/ different powers
   sumC = cz + cz2 * (bf[0] + cz * (bf[1] + sumC));

   const std::complex<double> result
      = double(jsgn) * sumC + cy + ipi12 * M_PI * M_PI / 12.;

   return result;
}

} // namespace gm2calc
