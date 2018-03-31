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

#include "dictionary/OpenSMOKE_DictionaryManager.h"
#include "dictionary/OpenSMOKE_DictionaryGrammar.h"
#include "dictionary/OpenSMOKE_DictionaryKeyWord.h"

#ifndef GRAMMAR_NETSMOKE_H
#define GRAMMAR_NETSMOKE_H

namespace NetSMOKE
{
	class Grammar_NetSMOKE : public OpenSMOKE::OpenSMOKE_DictionaryGrammar
	{
	protected:

		virtual void DefineRules()
		{
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@KineticsFolder", 
																OpenSMOKE::SINGLE_PATH, 
																"Name of the folder containing the kinetic scheme (XML Version)", 
																true,
																"@KineticsPreProcessor",
																"none",
																"none") );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@KineticsPreProcessor", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Name of the dictionary containing the list of kinetic files to be interpreted", 
																true,
																"@KineticsFolder",
																"none",
																"none") );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Inlets", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Name of the dictionary containing the list of inlets to the network", 
																true,
																"none",
																"none",
																"none") );			
																
			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Reactors", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Name of the dictionary containing the list of reactors in the network", 
																true,
																"none",
																"none",
																"none") );			

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Mixers", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Name of the dictionary containing the list of mixers in the network", 
																false) );

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Splitters", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Name of the dictionary containing the list of splitters in the network", 
																false) );	

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@PhaseSplitters", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Name of the dictionary containing the list of phase splitters in the network", 
																false) );										

			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Options", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Dictionary containing additional options for solving the reactor network", 
																false) );


			AddKeyWord( OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@OdeParameters", 
																OpenSMOKE::SINGLE_DICTIONARY, 
																"Dictionary containing the numerical parameters for solving the stiff ODE system", 
																false) );	

		}
	};
} // End namespace

#endif /* GRAMMAR_NETSMOKE_H */

