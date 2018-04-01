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

#ifndef GRAMMAR_NETSMOKE_PHASESPLITTERS_H
#define	GRAMMAR_NETSMOKE_PHASESPLITTERS_H

#include <string>
#include "boost/filesystem.hpp"
#include "dictionary/OpenSMOKE_Dictionary.h"
#include "dictionary/OpenSMOKE_DictionaryGrammar.h"

namespace NetSMOKE
{

	class Grammar_NetSMOKE_PhaseSplitters : public OpenSMOKE::OpenSMOKE_DictionaryGrammar
	{
	protected:

		virtual void DefineRules()
		{
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@PhaseSplitter", 
																OpenSMOKE::SINGLE_STRING, 
																"Name and declaration of this phase splitter (i.e. PS1)", 
																true) );	
			
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("OutletPhase", 
																OpenSMOKE::VECT_STRING, 
																"Phase of the outlets ordered as the OutletStreams vector", 
																true) );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("OutletStream", 
																OpenSMOKE::VECT_INT, 
																"ID numbers of the outlets ordered as the OutletPhase vector", 
																true) );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("InletStream", 
																OpenSMOKE::SINGLE_INT, 
																"ID number of the inlet stream", 
																true) );	
															
		}
	};

	void GetNumberOfPhaseSplitters()
	{
	}

	void GetPhaseSplitterDataFromDictionary(OpenSMOKE::OpenSMOKE_Dictionary& dictionary, std::vector<NetSMOKE::UnitInfo> &UnitsData)
	{
		NetSMOKE::UnitInfo TempUnit;

		std::string name;
		dictionary.ReadString("@PhaseSplitter", name);
		TempUnit.name = name;

		std::vector<std::string> phases;
		dictionary.ReadOption("OutletPhase", name);
		TempUnit.outlet_phase = phases;

		std::vector<int> outlets;
		dictionary.ReadOption("OutletStream", outlets);
		TempUnit.outlets = outlets;

		int inlet;
		dictionary.ReadInt("InletStream", inlet);
		TempUnit.inlets = inlet;

		UnitsData.push_back(TempUnit);
		
	}
} // End namespace

#endif	/* GRAMMAR_NETSMOKE_PHASESPLITTERS_H */

