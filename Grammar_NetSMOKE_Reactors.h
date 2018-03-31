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
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Reactor", 
																OpenSMOKE::SINGLE_STRING, 
																"Name and declaration of this reactor", 
																true,
																"None",
																"None",
																"None") );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Type", 
																OpenSMOKE::SINGLE_STRING, 
																"Type of reactor (PSR or PFR)", 
																true,
																"None",
																"None",
																"None") );
			
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Phase", 
																OpenSMOKE::SINGLE_STRING, 
																"Phase of reactor", 
																true,
																"None",
																"None",
																"None") );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Energy", 
																OpenSMOKE::SINGLE_STRING, 
																"Energy type of reactor (i.e. Isothermal, Adiabatic, HeatExchange)", 
																true,
																"None",
																"None",
																"None") );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Pressure", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Pressure of the reactor (i.e. 1 atm)", 
																true,
																"None",
																"None",
																"None") );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Temperature", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Pressure of the reactor (i.e. 1 atm)", 
																true,
																"Temperature",
																"UA",
																"None") );
			
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("UA", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Global exchange coefficient multiplied for area (W/K)", 
																true,
																"Temperature",
																"GlobalExchangeCoefficient",
																"None") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("GlobalExchangeCoefficient", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Global exchange coefficient (W/m2/K)", 
																true,
																"Temperature",
																"UA",
																"None") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("ResidenceTime", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Residence time of the reactor (i.e. 1 s)", 
																true,
																"Volume",
																"Diameter",
																"Length") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Volume", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Volume of the reactor", 
																true,
																"ResidenceTime",
																"None",
																"None") );		

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Diameter", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Pressure of the reactor (i.e. 1 atm)", 
																true,
																"ResidenceTime",
																"None",
																"None") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Length", 
																OpenSMOKE::SINGLE_MEASURE, 
																"Pressure of the reactor (i.e. 1 atm)", 
																true,
																"ResidenceTime",
																"None",
																"None") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Inlet", 
																OpenSMOKE::SINGLE_INT, 
																"ID number of the inlet stream", 
																true,
																"None",
																"None",
																"None") );																																																		

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("Outlet", 
																OpenSMOKE::SINGLE_INT, 
																"ID number of the outlet stream", 
																true,
																"None",
																"None",
																"None") );	
		}
	};

	void GetNumbersOfReactors()
	{

	};

	void GetReactorsDataFromDictionary(OpenSMOKE::OpenSMOKE_Dictionary& dictionary, std::vector<NetSMOKE::UnitInfo> &UnitsData)
	{
		
		NetSMOKE::UnitInfo TempUnit;

		// Get name and type
		{
			if (dictionary.CheckOption("@Reactor") == true)
			{
				std::string name;
				dictionary.ReadString("@Reactor", name);
				TempUnit.name = name;
			}

			if (dictionary.CheckOption("Type") == true)
			{
				std::string type;
				dictionary.ReadString("@Reactor", type);
				TempUnit.tag = type;
			}

			if (dictionary.CheckOption("Energy") == true)
			{
				std::string energy;
				dictionary.ReadString("Energy", energy);
				TempUnit.energy = energy;
			}
		}

		// Energy informations
		if (TempUnit.energy == "Isothermal")
		{
			if (dictionary.CheckOption("Temperature") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("Temperature", value, units);
		
				if (units == "K")			TempUnit.temperature = value;
				else if (units == "C")		TempUnit.temperature = value + 273.15;
				else OpenSMOKE::FatalErrorMessage("Unknown temperature units");

			}
			else OpenSMOKE::FatalErrorMessage("Missing temperature keyword for reactor " + TempUnit.name);
		}
		else if (TempUnit.energy == "Adiabatic")
		{
			TempUnit.UA = 0.;
		}
		else if (TempUnit.energy == "HeatExchanger")
		{
			if (dictionary.CheckOption("UA") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("UA", value, units);
		
				if (units == "W/K")			TempUnit.UA = value;
				else OpenSMOKE::FatalErrorMessage("Unknown temperature units, use W/K");

			}
			else OpenSMOKE::FatalErrorMessage("Missing UA keyword for reactor " + TempUnit.name);
		}

		// Pressure
		{
			if (dictionary.CheckOption("Pressure") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("Pressure", value, units);

				if (units == "Pa")			TempUnit.pressure = value;
				else if (units == "bar")	TempUnit.pressure = value*1.e5;
				else if (units == "atm")	TempUnit.pressure = value*101325.;
				else OpenSMOKE::FatalErrorMessage("Unknown pressure units");
			}
		}

		// Residence time and geometry
		if (dictionary.CheckOption("ResidenceTime") == true)
		{
			double value;
			std::string units;
			dictionary.ReadMeasure("ResidenceTime", value, units);

			if (units == "s")			TempUnit.residence_time = value;
			else if (units == "min")	TempUnit.residence_time = value*60;
			else if (units == "hr")		TempUnit.residence_time = value*3600;
			else OpenSMOKE::FatalErrorMessage("Unknown residence time units");

			if (TempUnit.type == "PSR")
				TempUnit.volume = -1;
			else if (TempUnit.type == "PFR")
				TempUnit.diameter = -1;
				TempUnit.length = -1;
		}
		else if (TempUnit.type == "PSR")
		{
			if (dictionary.CheckOption("Volume") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("Volume", value, units);

				if (units == "m3")			TempUnit.volume = value;
				else if (units == "cm3")	TempUnit.volume = value/1000000;
				else if (units == "l")		TempUnit.volume = value/1000;
				else OpenSMOKE::FatalErrorMessage("Unknown volume units (use m3,cm3,l)");
				TempUnit.residence_time = -1;
			}
		}
		else if (TempUnit.type == "PFR")
		{
			if (dictionary.CheckOption("Diameter") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("Diameter", value, units);

				if (units == "m")			TempUnit.diameter = value;
				else if (units == "cm")	TempUnit.diameter = value/100;
				else OpenSMOKE::FatalErrorMessage("Unknown diameter units (use m,cm)");
				TempUnit.residence_time = -1;
			}
			else OpenSMOKE::FatalErrorMessage("Missing diameter for reactor " + TempUnit.name);

			if (dictionary.CheckOption("Length") == true)
			{
				double value;
				std::string units;
				dictionary.ReadMeasure("Length", value, units);

				if (units == "m")			TempUnit.length = value;
				else if (units == "cm")	TempUnit.length = value/100;
				else OpenSMOKE::FatalErrorMessage("Unknown length units (use m,cm)");
				TempUnit.residence_time = -1;
			}
			else OpenSMOKE::FatalErrorMessage("Missing length for reactor " + TempUnit.name);
		}
		
		// Inlets and outlets
		if (dictionary.CheckOption("Inlet") == true)
		{
			double value;
			dictionary.ReadInt("Inlet", value);
			TempUnit.inlets.push_back();
		}
		if (dictionary.CheckOption("Outlet") == true)
		{
			double value;
			dictionary.ReadInt("Outlet", value);
			TempUnit.outlets.push_back();
		}

	}

} // End namespace NetSMOKE

#endif	/* GRAMMAR_NETSMOKE_REACTORS_H */

