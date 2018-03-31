/*-----------------------------------------------------------------------*\
|																		  |
|			 _   _      _    _____ __  __  ____  _  ________         	  |
|			| \ | |    | |  / ____|  \/  |/ __ \| |/ /  ____|        	  |
|			|  \| | ___| |_| (___ | \  / | |  | | ' /| |__   			  |
|			| . ` |/ _ \ __|\___ \| |\/| | |  | |  < |  __|  		  	  |
|			| |\  |  __/ |_ ____) | |  | | |__| | . \| |____ 		 	  |
|			|_| \_|\___|\__|_____/|_|  |_|\____/|_|\_\______|		 	  |
|                                                                         |
|   Author: Matteo Mensi <matteo.mensi@mail.polimi.it>                    |
|   CRECK Modeling Group <http://creckmodeling.chem.polimi.it>            |
|   Department of Chemistry, Materials and Chemical Engineering           |
|   Politecnico di Milano                                                 |
|   P.zza Leonardo da Vinci 32, 20133 Milano                              |
|                                                                         |
\*-----------------------------------------------------------------------*/

#ifndef GRAMMAR_NETSMOKE_REACTORS_H
#define	GRAMMAR_NETSMOKE_REACTORS_H

#include <string>
#include "boost/filesystem.hpp"
#include "dictionary/OpenSMOKE_Dictionary.h"
#include "dictionary/OpenSMOKE_DictionaryGrammar.h"

namespace NetSMOKE
{

	class Grammar_NetSMOKE_Reactors : public OpenSMOKE::OpenSMOKE_DictionaryGrammar
	{
	protected:

		virtual void DefineRules()
		{
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Temperature", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Temperature of the mixture (i.e. 500 K)", 
																false) );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Pressure", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Pressure of the mixture (i.e. 1 atm)", 
																false) );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Density", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Density of the mixture (i.e. 1 g/cm3)", 
																false) );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@MoleFractions", 
																OpenSMOKE::VECTOR_STRING_DOUBLE, 
																"Mole fractions of the mixture (i.e. CH4 0.60 H2 0.40)", 
																true,
																"@MassFractions @Moles @Masses @EquivalenceRatio",
																"none",
																"none") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@MassFractions", 
																OpenSMOKE::VECTOR_STRING_DOUBLE, 
																"Mass fractions of the mixture (i.e. CH4 0.60 H2 0.40)", 
																true,
																"@MoleFractions @Moles @Masses @EquivalenceRatio",
																"none",
																"none") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Moles", 
																OpenSMOKE::VECTOR_STRING_DOUBLE, 
																"Moles (relative) of the mixture (i.e. CH4 2 H2 1)", 
																true,
																"@MoleFractions @MassFractions @Masses @EquivalenceRatio",
																"none",
																"none") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Masses", 
																OpenSMOKE::VECTOR_STRING_DOUBLE, 
																"Masses (relative) of the mixture (i.e. CH4 2 H2 1)", 
																true,
																"@MoleFractions @MassFractions @Moles @EquivalenceRatio",
																"none",
																"none") );


			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@FuelMoles", 
																OpenSMOKE::VECTOR_STRING_DOUBLE, 
																"Fuel moles", 
																false,
																"none",
																"@EquivalenceRatio",
																"@FuelMoleFractions @FuelMassFractions @FuelMasses") );

															
		}
	};

	template<typename Thermodynamics>
	void GetGasStatusFromDictionary(OpenSMOKE::OpenSMOKE_Dictionary& dictionary, Thermodynamics& thermodynamicsMapXML,
									double& T, double& P_Pa, OpenSMOKE::OpenSMOKEVectorDouble& omega)
	{
		Grammar_GasStatus grammar_gas_status;
		dictionary.SetGrammar(grammar_gas_status);

		unsigned int state_variables = 0;
		bool temperature_assigned = false;
		bool pressure_assigned = false;
		bool density_assigned = false;
		
		// Temperature
		{
			if (dictionary.CheckOption("@Temperature") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("@Temperature", value, units);
		
				if (units == "K")			T = value;
				else if (units == "C")		T = value + 273.15;
				else OpenSMOKE::FatalErrorMessage("Unknown temperature units");

				state_variables++;
				temperature_assigned = true;
			}
		}

		// Pressure
		{
			if (dictionary.CheckOption("@Pressure") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("@Pressure", value, units);

				if (units == "Pa")			P_Pa = value;
				else if (units == "bar")	P_Pa = value*1.e5;
				else if (units == "atm")	P_Pa = value*101325.;
				else OpenSMOKE::FatalErrorMessage("Unknown pressure units");

				state_variables++;
				pressure_assigned = true;
			}
		}

		// Density
		double rho;
		{
			if (dictionary.CheckOption("@Density") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("@Density", value, units);

				if (units == "kg/m3")		rho = value;
				else if (units == "g/cm3")	rho = value*1.e3;
				else OpenSMOKE::FatalErrorMessage("Unknown density units");

				state_variables++;
				density_assigned = true;
			}
		}

		if (state_variables != 2)
			OpenSMOKE::FatalErrorMessage("The status of a gas mixture requires any 2 (and only 2) among: @Temperature, @Pressure and @Density");

		// Composition
		{
			{
				std::vector<std::string> names;
				std::vector<double> values;
		
				if (dictionary.CheckOption("@MoleFractions") == true)
				{
					dictionary.ReadOption("@MoleFractions", names, values);
				
					const double sum =std::accumulate(values.begin(),values.end(),0.);
					if (sum<(1.-1e-6) || sum>(1.+1e-6))
						OpenSMOKE::FatalErrorMessage("The mole fractions must sum to 1.");

					OpenSMOKE::OpenSMOKEVectorDouble x(thermodynamicsMapXML.NumberOfSpecies());
					for(unsigned int i=0;i<names.size();i++)
						x[thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i]/sum;
					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega, true);
					double MW;
					thermodynamicsMapXML.MassFractions_From_MoleFractions(omega.GetHandle(),MW,x.GetHandle());
				}
				else if (dictionary.CheckOption("@MassFractions") == true)
				{
					dictionary.ReadOption("@MassFractions", names, values);

					const double sum =std::accumulate(values.begin(),values.end(),0.);
					if (sum<(1.-1e-6) || sum>(1.+1e-6))
						OpenSMOKE::FatalErrorMessage("The mass fractions must sum to 1.");

					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega, true);
					for(unsigned int i=0;i<names.size();i++)
						omega[thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i]/sum;
				}
				else if (dictionary.CheckOption("@Moles") == true)
				{
					dictionary.ReadOption("@Moles", names, values);
					const double sum =std::accumulate(values.begin(),values.end(),0.);
					OpenSMOKE::OpenSMOKEVectorDouble x(thermodynamicsMapXML.NumberOfSpecies());
					for(unsigned int i=0;i<names.size();i++)
						x[thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i]/sum;
					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega, true);
					double MW;
					thermodynamicsMapXML.MassFractions_From_MoleFractions(omega.GetHandle(),MW,x.GetHandle());
				}
				else if (dictionary.CheckOption("@Masses") == true)
				{
					dictionary.ReadOption("@Masses", names, values);
					const double sum =std::accumulate(values.begin(),values.end(),0.);
					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega, true);
					for(unsigned int i=0;i<names.size();i++)
						omega[thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i]/sum;
				}
			}
		}

		if (density_assigned == true)
		{
			const double MW = thermodynamicsMapXML.MolecularWeight_From_MassFractions(omega.GetHandle());
			if (temperature_assigned == true)	P_Pa	= rho*PhysicalConstants::R_J_kmol*T/MW;
			if (pressure_assigned == true)		T		= P_Pa*MW/PhysicalConstants::R_J_kmol/rho;
		}
	}

	template<typename Thermodynamics>
	void GetGasStatusFromDictionary(OpenSMOKE::OpenSMOKE_Dictionary& dictionary, Thermodynamics& thermodynamicsMapXML,
		std::vector<double>& T, std::vector<double>& P_Pa, std::vector<OpenSMOKE::OpenSMOKEVectorDouble>& omega, std::vector<double>& equivalence_ratios)
	{
		Grammar_GasStatus grammar_gas_status;
		dictionary.SetGrammar(grammar_gas_status);

		unsigned int state_variables = 0;
		bool temperature_assigned = false;
		bool pressure_assigned = false;
		bool density_assigned = false;

		// Temperature
		{
			T.resize(1);
			if (dictionary.CheckOption("@Temperature") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("@Temperature", value, units);

				if (units == "K")			T[0] = value;
				else if (units == "C")		T[0] = value + 273.15;
				else OpenSMOKE::FatalErrorMessage("Unknown temperature units");

				state_variables++;
				temperature_assigned = true;
			}
		}

		// Pressure
		{
			P_Pa.resize(1);
			if (dictionary.CheckOption("@Pressure") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("@Pressure", value, units);

				if (units == "Pa")			P_Pa[0] = value;
				else if (units == "bar")	P_Pa[0] = value*1.e5;
				else if (units == "atm")	P_Pa[0] = value*101325.;
				else OpenSMOKE::FatalErrorMessage("Unknown pressure units");

				state_variables++;
				pressure_assigned = true;
			}
		}

		// Density
		double rho;
		{
			if (dictionary.CheckOption("@Density") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("@Density", value, units);

				if (units == "kg/m3")		rho = value;
				else if (units == "g/cm3")	rho = value*1.e3;
				else OpenSMOKE::FatalErrorMessage("Unknown density units");

				state_variables++;
				density_assigned = true;
			}
		}

		if (state_variables != 2)
			OpenSMOKE::FatalErrorMessage("The status of a gas mixture requires any 2 (and only 2) among: @Temperature, @Pressure and @Density");

		// Composition
		{
			{
				std::vector<std::string> names;
				std::vector<double> values;

				if (dictionary.CheckOption("@MoleFractions") == true)
				{
					omega.resize(1);

					dictionary.ReadOption("@MoleFractions", names, values);

					const double sum = std::accumulate(values.begin(), values.end(), 0.);
					if (sum<(1. - 1e-6) || sum>(1. + 1e-6))
						OpenSMOKE::FatalErrorMessage("The mole fractions must sum to 1.");

					OpenSMOKE::OpenSMOKEVectorDouble x(thermodynamicsMapXML.NumberOfSpecies());
					for (unsigned int i = 0; i<names.size(); i++)
						x[thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i] / sum;
					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega[0], true);
					double MW;
					thermodynamicsMapXML.MassFractions_From_MoleFractions(omega[0].GetHandle(), MW, x.GetHandle());
				}
				else if (dictionary.CheckOption("@MassFractions") == true)
				{
					omega.resize(1);

					dictionary.ReadOption("@MassFractions", names, values);

					const double sum = std::accumulate(values.begin(), values.end(), 0.);
					if (sum<(1. - 1e-6) || sum>(1. + 1e-6))
						OpenSMOKE::FatalErrorMessage("The mass fractions must sum to 1.");

					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega[0], true);
					for (unsigned int i = 0; i<names.size(); i++)
						omega[0][thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i] / sum;
				}
				else if (dictionary.CheckOption("@Moles") == true)
				{
					omega.resize(1);

					dictionary.ReadOption("@Moles", names, values);
					const double sum = std::accumulate(values.begin(), values.end(), 0.);
					OpenSMOKE::OpenSMOKEVectorDouble x(thermodynamicsMapXML.NumberOfSpecies());
					for (unsigned int i = 0; i<names.size(); i++)
						x[thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i] / sum;
					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega[0], true);
					double MW;
					thermodynamicsMapXML.MassFractions_From_MoleFractions(omega[0].GetHandle(), MW, x.GetHandle());
				}
				else if (dictionary.CheckOption("@Masses") == true)
				{
					omega.resize(1);
					dictionary.ReadOption("@Masses", names, values);
					const double sum = std::accumulate(values.begin(), values.end(), 0.);
					ChangeDimensions(thermodynamicsMapXML.NumberOfSpecies(), &omega[0], true);
					for (unsigned int i = 0; i<names.size(); i++)
						omega[0][thermodynamicsMapXML.IndexOfSpecies(names[i])] = values[i] / sum;
				}
			}
		}

		if (density_assigned == true)
		{
			if (omega.size() != 1)
				OpenSMOKE::FatalErrorMessage("The multiple @EquivalenceRatio option cannot be used together with the @Density option.");
			else
			{
				const double MW = thermodynamicsMapXML.MolecularWeight_From_MassFractions(omega[0].GetHandle());
				if (temperature_assigned == true)	P_Pa[0] = rho*PhysicalConstants::R_J_kmol*T[0] / MW;
				if (pressure_assigned == true)		T[0] = P_Pa[0] * MW / PhysicalConstants::R_J_kmol / rho;
			}
		}

		// TOADJUST
		{
			const double T_ = T[0];
			const double P_Pa_ = P_Pa[0];
			T.resize(omega.size());
			P_Pa.resize(omega.size());
			for (unsigned int k = 0; k < omega.size(); k++)
			{
				T[k] = T_;
				P_Pa[k] = P_Pa_;
			}
		}
	}
}

#endif	/* GRAMMAR_NETSMOKE_REACTORS_H */

