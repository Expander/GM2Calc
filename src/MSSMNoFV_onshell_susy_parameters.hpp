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

#ifndef MSSMNoFV_onshell_susy_parameters_H
#define MSSMNoFV_onshell_susy_parameters_H

#include <iosfwd>
#include <Eigen/Core>

namespace flexiblesusy {
namespace gm2calc {

class MSSMNoFV_onshell_susy_parameters {
public:
   explicit MSSMNoFV_onshell_susy_parameters();
   MSSMNoFV_onshell_susy_parameters(double scale_,
      const Eigen::Matrix<double,3,3>& Yd_, const Eigen::Matrix<double,3,3>& Ye_,
      const Eigen::Matrix<double,3,3>& Yu_, double Mu_, double g1_, double g2_,
      double g3_, double vd_, double vu_
   );
   virtual ~MSSMNoFV_onshell_susy_parameters() {}
   virtual void print(std::ostream&) const;
   virtual void clear();

   void set_scale(double s) { scale = s; }
   double get_scale() const { return scale; }

   void set_Yd(const Eigen::Matrix<double,3,3>& Yd_) { Yd = Yd_; }
   void set_Yd(int i, int k, double value) { Yd(i,k) = value; }
   void set_Ye(const Eigen::Matrix<double,3,3>& Ye_) { Ye = Ye_; }
   void set_Ye(int i, int k, double value) { Ye(i,k) = value; }
   void set_Yu(const Eigen::Matrix<double,3,3>& Yu_) { Yu = Yu_; }
   void set_Yu(int i, int k, double value) { Yu(i,k) = value; }
   void set_Mu(double Mu_) { Mu = Mu_; }
   void set_g1(double g1_) { g1 = g1_; }
   void set_g2(double g2_) { g2 = g2_; }
   void set_g3(double g3_) { g3 = g3_; }
   void set_vd(double vd_) { vd = vd_; }
   void set_vu(double vu_) { vu = vu_; }

   const Eigen::Matrix<double,3,3>& get_Yd() const { return Yd; }
   double get_Yd(int i, int k) const { return Yd(i,k); }
   const Eigen::Matrix<double,3,3>& get_Ye() const { return Ye; }
   double get_Ye(int i, int k) const { return Ye(i,k); }
   const Eigen::Matrix<double,3,3>& get_Yu() const { return Yu; }
   double get_Yu(int i, int k) const { return Yu(i,k); }
   double get_Mu() const { return Mu; }
   double get_g1() const { return g1; }
   double get_g2() const { return g2; }
   double get_g3() const { return g3; }
   double get_vd() const { return vd; }
   double get_vu() const { return vu; }

protected:
   double scale;
   Eigen::Matrix<double,3,3> Yd;
   Eigen::Matrix<double,3,3> Ye;
   Eigen::Matrix<double,3,3> Yu;
   double Mu;
   double g1;
   double g2;
   double g3;
   double vd;
   double vu;

private:
   static const int numberOfParameters = 33;
};

std::ostream& operator<<(std::ostream&, const MSSMNoFV_onshell_susy_parameters&);

} // namespace gm2calc
} // namespace flexiblesusy

#endif
