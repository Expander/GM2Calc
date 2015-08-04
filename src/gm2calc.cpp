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

#include "gm2_1loop.hpp"
#include "gm2_2loop.hpp"
#include "error.hpp"
#include "config.h"

#include "gm2_slha_io.hpp"
#include "MSSMNoFV_onshell.hpp"

#include <iostream>
#include <limits>

#define ERROR(message) std::cerr << "Error: " << message << '\n';
#define WARNING(message) std::cerr << "Warning: " << message << '\n';

using namespace flexiblesusy;

struct Gm2_cmd_line_options {
   enum E_input_type { SLHA, GM2Calc };

   std::string input_source;
   E_input_type input_type;

   static bool starts_with(const std::string& str, const std::string& prefix) {
      return !str.compare(0, prefix.size(), prefix);
   }
};

void print_usage(const char* program_name)
{
   std::cout <<
      "Usage: " << program_name << " [options]\n"
      "Options:\n"
      "  --slha-input-file=<source>      SLHA input source (file name or - for stdin)\n"
      "  --gm2calc-input-file=<source>   GM2Calc input source (file name or - for stdin)\n"
      "  --help,-h                       print this help message\n"
      "  --version,-v                    print version number"
             << std::endl;
}

Gm2_cmd_line_options get_cmd_line_options(int argc, const char* argv[])
{
   Gm2_cmd_line_options options;

   for (int i = 1; i < argc; ++i) {
      const std::string option(argv[i]);

      if (Gm2_cmd_line_options::starts_with(option, "--slha-input-file=")) {
         options.input_source = option.substr(18);
         options.input_type = Gm2_cmd_line_options::SLHA;
         continue;
      }

      if (Gm2_cmd_line_options::starts_with(option, "--gm2calc-input-file=")) {
         options.input_source = option.substr(21);
         options.input_type = Gm2_cmd_line_options::GM2Calc;
         continue;
      }

      if (option == "--help" || option == "-h") {
         print_usage(argv[0]);
         exit(EXIT_SUCCESS);
      }

      if (option == "--version" || option == "-v") {
         std::cout << GM2CALC_VERSION << '\n';
         exit(EXIT_SUCCESS);
      }

      ERROR("Unrecognized command line option: " << option);
      exit(EXIT_FAILURE);
   }

   return options;
}

/**
 * Setup the model parameters consistently, depending on the input
 * parameter set (chosen by the user).
 *
 * If the user has chosen an SLHA-compliant input, the input
 * parameters (given in \a slha_io) are converted to the g-2 specific
 * on-shell scheme.
 *
 * If the user has chosen the GM2Calc specific input, the input
 * parameters are already given in the on-shell scheme.  In this case
 * the SUSY mass spectrum is calculated from the given input
 * parameters.
 *
 * @param model model parameters (and particle masses)
 * @param slha_io object with numerical values of input parameters
 * @param options command line options (defines meaning of input parameter set)
 */
void setup_model(gm2calc::MSSMNoFV_onshell& model,
                 const gm2calc::GM2_slha_io& slha_io,
                 const Gm2_cmd_line_options& options)
{
   switch (options.input_type) {
   case Gm2_cmd_line_options::SLHA:
      // determine on-shell model parameters from an SLHA parameter
      // set
      fill_slha(slha_io, model);
      model.convert_to_onshell();
      break;
   case Gm2_cmd_line_options::GM2Calc:
      // on-shell parameters are directly given, calculate mass
      // spectrum
      fill_gm2calc(slha_io, model);
      model.calculate_masses();
      break;
   default:
      throw SetupError("Unknown input option");
      break;
   }
}

/**
 * Calculate most precise value of a_mu for a given set op parameters
 */
double calculate_amu_best(gm2calc::MSSMNoFV_onshell& model)
{
   const double gm2_1l_tan_beta_resummed =
      gm2calc::calculate_amu_1loop(model);
   const double gm2_2l_tan_beta_resummed =
      + gm2calc::amu2LFSfapprox(model)
      + gm2calc::amuChipmPhotonic(model)
      + gm2calc::amuChi0Photonic(model);

   const double gm2_best = gm2_1l_tan_beta_resummed + gm2_2l_tan_beta_resummed;

   return gm2_best;
}

/**
 * Prints detailed a_mu calculation (1-loop w/ and w/o tan(beta)
 * resumation, 2-loop, and differenc contributions).
 */
void print_amu_detailed(const gm2calc::MSSMNoFV_onshell& model)
{
   const double gm2_1l =
      gm2calc::calculate_amu_1loop_non_tan_beta_resummed(model);
   const double gm2_1l_tan_beta_resummed =
      gm2calc::calculate_amu_1loop(model);
   const double gm2_2l_tan_beta_resummed =
      + gm2calc::amu2LFSfapprox(model)
      + gm2calc::amuChipmPhotonic(model)
      + gm2calc::amuChi0Photonic(model);
   const double gm2_2l_tanb_approx =
      + (gm2calc::tan_beta_cor(model) - 1.) * gm2_1l;

   const double gm2_best = gm2_1l_tan_beta_resummed + gm2_2l_tan_beta_resummed;

   std::cout << model << '\n';

   // set high output precision
   std::ios_base::fmtflags flags(std::cout.flags());
   std::cout.precision(std::numeric_limits<double>::digits10);

   std::cout <<
      "=============================================\n"
      "   amu (1-loop + 2-loop best) = " << gm2_best << '\n' <<
      "=============================================\n\n"

      "==============================\n"
      "   amu (1-loop) corrections\n"
      "==============================\n\n"

      "--------------------------------------\n"
      "amu (1-loop strict) = " << gm2_1l << '\n' <<
      "--------------------------------------\n"
      "amu (1-loop TB resummed) = " << gm2_1l_tan_beta_resummed << '\n' <<
      "--------------------------------------\n"
      "amuChi0 (TB resummed) = " << gm2calc::amuChi0(model) << '\n' <<
      "amuChipm (TB resummed) = " << gm2calc::amuChipm(model) << '\n' <<
      "--------------------------------------\n"
      "amu1Lapprox = " << amu1Lapprox(model) << '\n' <<
      "--------------------------------------\n"
      "amuWHnu = " << gm2calc::amuWHnu(model) << '\n' <<
      "amuBmuLmuR = " << gm2calc::amuBmuLmuR(model) << '\n' <<
      "amuBHmuL = " << gm2calc::amuBHmuL(model) << '\n' <<
      "amuWHmuL = " << gm2calc::amuWHmuL(model) << '\n' <<
      "amuBHmuR = " << gm2calc::amuBHmuR(model) << "\n\n" <<

      "==============================\n"
      "   amu (2-loop) corrections\n"
      "==============================\n\n"

      "--------------------------------------\n"
      "amu (2-loop (TB resummed)) = " << gm2_2l_tan_beta_resummed << '\n' <<
      "2-loop / 1-loop = " <<
      (100. * gm2_2l_tan_beta_resummed / gm2_1l_tan_beta_resummed) << " %\n"
      "--------------------------------------\n"
      "amu2LSFsapprox = " << gm2calc::amu2LFSfapprox(model) << '\n' <<
      "--------------------------------------\n"
      "amuWHnu2L = " << gm2calc::amuWHnu2L(model) << '\n' <<
      "amuWHmuL2L = " << gm2calc::amuWHmuL2L(model) << '\n' <<
      "amuBHmuL2L = " << gm2calc::amuBHmuL2L(model) << '\n' <<
      "amuBHmuR2L = " << gm2calc::amuBHmuR2L(model) << '\n' <<
      "amuBmuLmuR2L = " << gm2calc::amuBmuLmuR2L(model) << '\n' <<
      "2-loop_FSfapprox / 1-loop = " <<
      (100. * gm2calc::amu2LFSfapprox(model) / gm2_1l) << " %\n"
      "--------------------------------------\n"
      "tan(beta) correction = " << gm2_2l_tanb_approx << '\n' <<
      "2-loop_tan(beta) / 1-loop = " <<
      (100. * gm2_2l_tanb_approx / gm2_1l) << " %\n"
      "--------------------------------------\n"
      "amu2LPhotonic = " <<
      (gm2calc::amuChipmPhotonic(model)
       + gm2calc::amuChi0Photonic(model)) << '\n' <<
      "--------------------------------------\n"
      "amuChipmPhotonic = " << gm2calc::amuChipmPhotonic(model) << '\n' <<
      "amuChi0Photonic = " << gm2calc::amuChi0Photonic(model) << '\n' <<
      "2-loop_Photonic / 1-loop = " <<
      100. * (amuChipmPhotonic(model)
              + gm2calc::amuChi0Photonic(model)) / gm2_1l << " %\n"
      "--------------------------------------\n"
      "amu2LaSferm = " << gm2calc::amua2LSferm(model) << '\n' <<
      "amua2LaCha = " << gm2calc::amua2LCha(model) << '\n' <<
      "--------------------------------------\n"
      ;

   std::cout.flags(flags);
}

/**
 * Calculates a_mu for a given set of configuration options (loop
 * order, tan(beta) resummation.
 *
 * @param model model parameters
 * @param config_options configuration options
 *
 * @return a_mu
 */
double calculate_amu(gm2calc::MSSMNoFV_onshell& model,
                     const gm2calc::Config_options& config_options)
{
   double result = 0.;

   switch (config_options.loop_order) {
   case 0:
      ERROR("0-loop order not supported!");
      break;
   case 1:
      if (config_options.tanb_resummation)
         result = gm2calc::calculate_amu_1loop(model);
      else
         result = gm2calc::calculate_amu_1loop_non_tan_beta_resummed(model);
      break;
   case 2:
      if (!config_options.tanb_resummation)
         WARNING("tan(beta) resummation is always enabled at 2-loop level");
      // calculate amu w/ resummation
      result = gm2calc::calculate_amu_1loop(model)
         + gm2calc::amu2LFSfapprox(model)
         + gm2calc::amuChipmPhotonic(model)
         + gm2calc::amuChi0Photonic(model);
      break;
   default:
      ERROR("loop orders > 2 not supported!");
      break;
   }

   return result;
}

int main(int argc, const char* argv[])
{
   Gm2_cmd_line_options options(get_cmd_line_options(argc, argv));

   if (options.input_source.empty()) {
      ERROR("No input source given!\n"
            "   Please provide an SLHA input via the option --slha-input-file=\n"
            "   or a GM2Calc input via the option --gm2calc-input-file=");
      return EXIT_FAILURE;
   }

   gm2calc::MSSMNoFV_onshell model;
   gm2calc::GM2_slha_io slha_io;
   gm2calc::Config_options config_options;

   try {
      slha_io.read_from_source(options.input_source);
      fill(slha_io, config_options);
      setup_model(model, slha_io, options);
   } catch (const Error& error) {
      ERROR(error.what());
      return EXIT_FAILURE;
   }

   switch (config_options.output_format) {
   case gm2calc::Config_options::Minimal:
      std::cout << std::setprecision(std::numeric_limits<double>::digits10)
                << std::scientific
                << calculate_amu_best(model) << '\n';
      break;
   case gm2calc::Config_options::Detailed:
      print_amu_detailed(model);
      break;
   case gm2calc::Config_options::NMSSMTools:
      slha_io.fill_block_entry("LOWEN", 6,
                               calculate_amu(model, config_options),
                               "Delta(g-2)_muon/2");
      slha_io.write_to_stream(std::cout);
      break;
   case gm2calc::Config_options::SPheno:
      slha_io.fill_block_entry("SPhenoLowEnergy", 21,
                               calculate_amu(model, config_options),
                               "Delta(g-2)_muon/2");
      slha_io.write_to_stream(std::cout);
      break;
   default:
      ERROR("Unknown output format: " << config_options.output_format);
      return EXIT_FAILURE;
      break;
   }

   return 0;
}
