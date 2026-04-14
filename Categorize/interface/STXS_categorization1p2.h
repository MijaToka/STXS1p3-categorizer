#ifndef STXS_CATEGORIZATION_1P2_H
#define STXS_CATEGORIZATION_1P2_H

#include "STXSCategorizer/Categorize/interface/STXS_common.h"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <map>


namespace Version1_2 {
std::map<STXS1, ROOT::RDF::RNode>
second_categorization(std::map<STXS0, ROOT::RDF::RNode> arr);
}

#endif